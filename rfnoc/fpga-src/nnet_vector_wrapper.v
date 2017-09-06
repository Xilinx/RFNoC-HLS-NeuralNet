//
//    rfnoc-hls-neuralnet: Vivado HLS code for neural-net building blocks
//
//    Copyright (C) 2017 EJ Kreinar
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

// nnet_vector_wrapper is designed to read the network input and output sizes from
// a user-generated neural network.
//
// - The nnet_vector_wrapper will wrangle the tlasts and tusers to be
//   compatible with the rfnoc crossbar and the neural net HLS code
// - Also, provide a "samples-per-packet" settings register that lets you
//   manually set the output packet size.
//    -> This is recommended for SMALL network outputs (otherwise, it seems
//       the processor gets bogged down due to too many small packets)

// NOTE: Overall, I'm not really sure what EXACTLY the resizers are doing to tuser.
// But, I've done my best to maintain the tuser propagation through the packet_resizers
// There may be some EOB stuff that needs to carry through -- or something?

module nnet_vector_wrapper #(
  parameter WIDTH = 16,
  parameter HEADER_WIDTH = 128, 
  parameter HEADER_FIFO_SIZE = 3,
  parameter SR_USER_SPP = 131
)(
  input clk, input reset, input clear,
  input [15:0] next_dst_sid,
  input [15:0] nnet_size_in, nnet_size_out,
  output [15:0] spp_out,

  // Control Registers
  input [31:0] set_data, input [7:0] set_addr, input set_stb,

  // AXI Interface from axi_wrapper
  input [2*WIDTH-1:0] i_tdata, input i_tlast, input i_tvalid, output i_tready, input [HEADER_WIDTH-1:0] i_tuser,
  output [2*WIDTH-1:0] o_tdata, output o_tlast, output o_tvalid, input o_tready, output [HEADER_WIDTH-1:0] o_tuser,

  // AXI Interface to user code
  output [2*WIDTH-1:0] m_axis_data_tdata, output m_axis_data_tlast, output m_axis_data_tvalid, input m_axis_data_tready,
  input [2*WIDTH-1:0] s_axis_data_tdata, input s_axis_data_tlast, input s_axis_data_tvalid, output s_axis_data_tready
);

  wire [HEADER_WIDTH-1:0] m_axis_data_tuser;

  wire [2*WIDTH-1:0] int_tdata;
  wire [HEADER_WIDTH-1:0] int_tuser;
  wire int_tlast;
  wire int_tvalid;
  wire int_tready;


  // Create a little setting register to override SPP
  // At SR_USER_SPP, provide 15 bits of packet_length
  // On reset, "spp_reg" defaults to nnet_size_out. When spp_changed, use spp_user instead
  wire [15:0] spp_user;
  wire spp_changed;
  setting_reg #(
    .my_addr(SR_USER_SPP), .awidth(8), .width(16))
  sr_spp_reg (
    .clk(clk), .rst(reset | clear),
    .strobe(set_stb), .addr(set_addr), .in(set_data), .out(spp_user), .changed(spp_changed));

  reg [15:0] spp_reg;
  assign spp_out = spp_reg;
  always @(posedge clk)
  if(reset | clear)
    spp_reg <= nnet_size_out;
  else if(spp_changed)
    spp_reg <= spp_user;


  // Resize the input packet to the target length
  packet_resizer_variable inst_packet_resizer_in (
    .clk(clk), .reset(reset | clear),
    .next_dst_sid(next_dst_sid),
    .pkt_size(nnet_size_in),
    .i_tdata(i_tdata), .i_tuser(i_tuser),
    .i_tlast(i_tlast), .i_tvalid(i_tvalid), .i_tready(i_tready),
    .o_tdata(m_axis_data_tdata), .o_tuser(m_axis_data_tuser),
    .o_tlast(m_axis_data_tlast), .o_tvalid(m_axis_data_tvalid), .o_tready(m_axis_data_tready));

  reg sof_in  = 1'b1;
  reg sof_out = 1'b1;
  always @(posedge clk) begin
    if (reset | clear) begin
      sof_in     <= 1'b1;
      sof_out    <= 1'b1;
    end else begin
      if (m_axis_data_tvalid & m_axis_data_tready) begin
        if (m_axis_data_tlast) begin
          sof_in  <= 1'b1;
        end else begin
          sof_in  <= 1'b0;
        end
      end
      if (o_tvalid & o_tready) begin
        if (o_tlast) begin
          sof_out  <= 1'b1;
        end else begin
          sof_out  <= 1'b0;
        end
      end
    end
  end

  wire [127:0] hdr_tuser_int;
  wire hdr_tuser_valid = sof_in & m_axis_data_tvalid & m_axis_data_tready;
  wire hdr_tuser_ready = sof_out & o_tvalid & o_tready;

  // Store the input headers (tuser)
  axi_fifo #(
    .WIDTH(HEADER_WIDTH), .SIZE(HEADER_FIFO_SIZE))
  axi_fifo_header (
    .clk(clk), .reset(reset), .clear(clear),
    .i_tdata(m_axis_data_tuser),
    .i_tvalid(hdr_tuser_valid), .i_tready(),
    .o_tdata(hdr_tuser_int),
    .o_tvalid(), .o_tready(hdr_tuser_ready), // Consume header on last output sample
    .space(), .occupied());

  // This resizer pulls tuser out of the header FIFO
  packet_resizer_variable inst_packet_resizer_out (
    .clk(clk), .reset(reset | clear),
    .next_dst_sid(next_dst_sid),
    .pkt_size(nnet_size_out),
    .i_tdata(s_axis_data_tdata), .i_tuser(hdr_tuser_int),
    .i_tlast(s_axis_data_tlast), .i_tvalid(s_axis_data_tvalid), .i_tready(s_axis_data_tready),
    .o_tdata(int_tdata), .o_tuser(int_tuser),
    .o_tlast(int_tlast), .o_tvalid(int_tvalid), .o_tready(int_tready));

  // Resize to the desired SPP
  packet_resizer_variable inst_packet_resizer_out_variable (
    .clk(clk), .reset(reset | clear),
    .next_dst_sid(next_dst_sid),
    .pkt_size(spp_reg),
    .i_tdata(int_tdata), .i_tuser(int_tuser),
    .i_tlast(int_tlast), .i_tvalid(int_tvalid), .i_tready(int_tready),
    .o_tdata(o_tdata), .o_tuser(o_tuser),
    .o_tlast(o_tlast), .o_tvalid(o_tvalid), .o_tready(o_tready));

endmodule
