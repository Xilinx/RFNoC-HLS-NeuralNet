# RFNoC Directory

This is a very fun and important place... it contains all the FPGA code for this repo!

- hls4ml: This is a git submodule that points to [hls4ml github project](https://github.com/hls-fpga-machine-learning/hls4ml). If the hls4ml folder is empty, or you ever need to update, use the following syntax: `git submodule update --init` to re-initialize this repo to what is set up inside rfnoc-hls-neuralnet repo.
- [blocks](blocks): RFNoC XML block definitions
- [fpga-src](fpga-src): Verilog source files (.v) files for noc_blocks
- [hls](hls): Contains the C++ projects which are used with Vivado HLS. See the readme here for details.
- [ip](ip): Contains Xilinx-specific IP. See readme for details.
- [testbenches](testbenches): Provides systemverilog testbenches in the RFNoC workflow that can be used to exercise noc_blocks in fpga-src.

When building FPGA images from uhd-fpga's uhd_image_builder, make sure to include *this* directory as the include directory -- it should be populated in the Makefile.OOT.inc when you generate an image for a target (e.g., e310, x310, n3xx).
