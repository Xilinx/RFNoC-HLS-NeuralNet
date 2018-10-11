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
// - The nnet_vector_wrapper will wrangle the tlasts to be
//   compatible with the rfnoc crossbar and the neural net HLS code
// - For better or worse, this ignores tuser
// - Provides a "samples-per-packet" settings register that lets you
//   manually set the output packet size.
//    -> This is recommended for SMALL network outputs (otherwise, it seems
//       the processor gets bogged down due to too many small packets)
// - Performs bit-width conversion between the XBAR_WIDTH (32 bits default)
//   and desired width for the network
// - Network widths are specific in units of bytes (valid options: {1, 2, 4})
//   and are determined at compile time

module nnet_vector_wrapper #(
  parameter XBAR_WIDTH = 32,
  parameter NNET_BYTES_IN = 4,
  parameter NNET_BYTES_OUT = 4,
  parameter HEADER_WIDTH = 128,
  parameter SR_USER_SPP = 131
)(
  input clk, input reset, input clear,
  input [15:0] next_dst_sid, input [15:0] src_sid,
  input [15:0] nnet_size_in, nnet_size_out,
  output [15:0] spp_out,

  // Control Registers
  input [31:0] set_data, input [7:0] set_addr, input set_stb,

  // AXI Interface from axi_wrapper
  input [XBAR_WIDTH-1:0] i_tdata, input i_tlast, input i_tvalid, output i_tready, input [HEADER_WIDTH-1:0] i_tuser,
  output [XBAR_WIDTH-1:0] o_tdata, output o_tlast, output o_tvalid, input o_tready, output [HEADER_WIDTH-1:0] o_tuser,

  // AXI Interface to user code
  output [8*NNET_BYTES_IN-1:0] m_axis_data_tdata, output m_axis_data_tlast, output m_axis_data_tvalid, input m_axis_data_tready,
  input [8*NNET_BYTES_OUT-1:0] s_axis_data_tdata, input s_axis_data_tlast, input s_axis_data_tvalid, output s_axis_data_tready
);

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
  if(reset | clear) begin
    spp_reg <= NNET_BYTES_OUT == 4 ? nnet_size_out << 2 :
               NNET_BYTES_OUT == 2 ? nnet_size_out << 1 :
               nnet_size_out;
  end else if(spp_changed) begin
    spp_reg <= spp_user;
  end

  cvita_hdr_encoder cvita_hdr_encoder (
    .pkt_type(2'd0), .eob(1'b0), .has_time(1'b0),
    .seqnum(12'd0), .payload_length(spp_out), .dst_sid(next_dst_sid), .src_sid(src_sid),
    .vita_time(64'd0),
    .header(o_tuser));

  generate
    if (NNET_BYTES_IN == 1) begin
      axis_dwidth_converter_4B_to_1B dwidth_converter_in (
        .aclk(clk),
        .aresetn(reset | clear),
        .s_axis_tdata(i_tdata),
        .s_axis_tlast(i_tlast),
        .s_axis_tvalid(i_tvalid),
        .s_axis_tready(i_tready),
        .m_axis_tdata(m_axis_data_tdata),
        .m_axis_tlast(m_axis_data_tlast),
        .m_axis_tvalid(m_axis_data_tvalid),
        .m_axis_tready(m_axis_data_tready)
      );
    end else if (NNET_BYTES_IN == 2) begin
      axis_dwidth_converter_4B_to_2B dwidth_converter_in (
        .aclk(clk),
        .aresetn(reset | clear),
        .s_axis_tdata(i_tdata),
        .s_axis_tlast(i_tlast),
        .s_axis_tvalid(i_tvalid),
        .s_axis_tready(i_tready),
        .m_axis_tdata(m_axis_data_tdata),
        .m_axis_tlast(m_axis_data_tlast),
        .m_axis_tvalid(m_axis_data_tvalid),
        .m_axis_tready(m_axis_data_tready)
      );
    end else begin
      assign m_axis_data_tdata = i_tdata;
      assign m_axis_data_tlast = i_tlast;
      assign m_axis_data_tvalid = i_tvalid;
      assign i_tready = m_axis_data_tready;
    end
  endgenerate


  generate
    if (NNET_BYTES_OUT == 1) begin
      axis_dwidth_converter_1B_to_4B dwidth_converter_out (
        .aclk(clk),
        .aresetn(reset | clear),
        .s_axis_tdata(s_axis_data_tdata),
        .s_axis_tlast(s_axis_data_tlast),
        .s_axis_tvalid(s_axis_data_tvalid),
        .s_axis_tready(s_axis_data_tready),
        .m_axis_tdata(o_tdata),
        .m_axis_tlast(o_tlast),
        .m_axis_tvalid(o_tvalid),
        .m_axis_tready(o_tready)
      );
    end else if (NNET_BYTES_OUT == 2) begin
      axis_dwidth_converter_2B_to_4B dwidth_converter_out (
        .aclk(clk),
        .aresetn(reset | clear),
        .s_axis_tdata(s_axis_data_tdata),
        .s_axis_tlast(s_axis_data_tlast),
        .s_axis_tvalid(s_axis_data_tvalid),
        .s_axis_tready(s_axis_data_tready),
        .m_axis_tdata(o_tdata),
        .m_axis_tlast(o_tlast),
        .m_axis_tvalid(o_tvalid),
        .m_axis_tready(o_tready)
      );
    end else begin
      assign o_tdata = s_axis_data_tdata;
      assign o_tlast = s_axis_data_tlast;
      assign o_tvalid = s_axis_data_tvalid;
      assign s_axis_data_tready = o_tready;
    end
  endgenerate

endmodule
