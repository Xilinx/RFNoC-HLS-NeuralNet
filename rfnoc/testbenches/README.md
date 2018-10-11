# RFNoC Testbenches

This is a typical RFNoC testbench folder.

Make sure to follow the [instructions](../../README.md) to pull the uhd-fpga patch into your uhd-fpga repo so HLS will build correctly.

The process to run a testbench is very simple:

1. Enter directory
2. `make xsim_hls`
3. If desired to debug via the Vivado GUI: `make xsim_hls GUI=1`
