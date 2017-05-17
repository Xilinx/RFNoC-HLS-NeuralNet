//
// Copyright 2016 Ettus Research
//
module nnet_vector_wrapper #(
  parameter WIDTH = 16,
  parameter HEADER_WIDTH = 128, 
  parameter HEADER_FIFO_SIZE = 3,
  parameter SR_SIZE_INPUT = 129,
  parameter SR_SIZE_OUTPUT = 130
)(
  input clk, input reset, input clear,
  input [15:0] next_dst_sid,
  input set_stb, input [7:0] set_addr, input [31:0] set_data,

  // AXI Interface from axi_wrapper
  input [2*WIDTH-1:0] i_tdata, input i_tlast, input i_tvalid, output i_tready, input [HEADER_WIDTH-1:0] i_tuser,
  output [2*WIDTH-1:0] o_tdata, output o_tlast, output o_tvalid, input o_tready, output [HEADER_WIDTH-1:0] o_tuser,

  // AXI Interface to user code
  output [2*WIDTH-1:0] m_axis_data_tdata, output m_axis_data_tlast, output m_axis_data_tvalid, input m_axis_data_tready,
  input [2*WIDTH-1:0] s_axis_data_tdata, input s_axis_data_tlast, input s_axis_data_tvalid, output s_axis_data_tready
);

  wire [HEADER_WIDTH-1:0] m_axis_data_tuser;

  packet_resizer #(
    .SR_PKT_SIZE(SR_SIZE_INPUT))
  inst_packet_resizer_in (
    .clk(clk), .reset(reset | clear),
    .next_dst_sid(next_dst_sid),
    .set_stb(set_stb), .set_addr(set_addr), .set_data(set_data),
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

  axi_fifo #(
    .WIDTH(HEADER_WIDTH), .SIZE(HEADER_FIFO_SIZE))
  axi_fifo_header (
    .clk(clk), .reset(reset), .clear(clear),
    .i_tdata(m_axis_data_tuser),
    .i_tvalid(hdr_tuser_valid), .i_tready(),
    .o_tdata(hdr_tuser_int),
    .o_tvalid(), .o_tready(hdr_tuser_ready), // Consume header on last output sample
    .space(), .occupied());


  packet_resizer #(
    .SR_PKT_SIZE(SR_SIZE_OUTPUT))
  inst_packet_resizer_out (
    .clk(clk), .reset(reset | clear),
    .next_dst_sid(next_dst_sid),
    .set_stb(set_stb), .set_addr(set_addr), .set_data(set_data),
    .i_tdata(s_axis_data_tdata), .i_tuser(hdr_tuser_int),
    .i_tlast(s_axis_data_tlast), .i_tvalid(s_axis_data_tvalid), .i_tready(s_axis_data_tready),
    .o_tdata(o_tdata), .o_tuser(o_tuser),
    .o_tlast(o_tlast), .o_tvalid(o_tvalid), .o_tready(o_tready));


endmodule
