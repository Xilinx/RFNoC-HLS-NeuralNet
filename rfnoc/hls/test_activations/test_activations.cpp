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

#include "test_activations.h"
#include "nnet_activation.h"


void test_activations(
      hls::stream<data_t>    &data,
      hls::stream<result_t>  &result)
{
    // Remove ap ctrl ports (ap_start, ap_ready, ap_idle, etc) since we only use the AXI-Stream ports
    #pragma HLS INTERFACE ap_ctrl_none port=return

    // ****************************************
    // NETWORK INSTATIATION
    // ****************************************

    // Activation function
//    nnet::sigmoid<data_t, result_t, N_LAYER, 256>(data, result);
    nnet::tanh<data_t, result_t, N_LAYER>(data, result);
}
