############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2015 Xilinx Inc. All rights reserved.
############################################################
open_project -reset test_activations_prj
set_top test_activations
add_files test_activations.cpp -cflags "-I[file normalize ../nnet_lib]"
add_files -tb test_activations_test.cpp -cflags "-I[file normalize ../nnet_lib]"
open_solution -reset "solution1"
set_part {xc7z020clg484-3}
create_clock -period 10 -name default
exit
