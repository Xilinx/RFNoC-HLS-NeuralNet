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

#include "nnet_conv.h"
#include "test_conv.h"

//*******************************************************
// Demonstration Weights/Biases
// (hardcoded)
//*******************************************************

weight_t test1_weights[TEST1_N_FILT][2*TEST1_CHAN_OUT] =
          { 1, 1,
            1, 1,
            1, 1,
            1, 1 };

bias_t   test1_biases[TEST1_CHAN_OUT] = { 0.5 };


weight_t test2_weights[TEST2_N_FILT][2*TEST2_CHAN_OUT] =
          { 1, 2, 3, 1, 2, 3,
            1, 2, 3, 1, 2, 3,
            1, 2, 3, 1, 2, 3,
            1, 2, 3, 1, 2, 3 };

bias_t   test2_biases[TEST2_CHAN_OUT] = { 0.25, 0.5, 0.75};


void get_test3(weight_t test3_weights[TEST3_N_FILT][2*TEST3_CHAN_OUT], bias_t test3_biases[TEST3_CHAN_OUT]){
  for (int ii=0; ii<TEST3_N_FILT; ii++){
    for (int jj=0; jj<TEST3_CHAN_OUT; jj++){
      test3_weights[ii][2*jj]=1.0;
      test3_weights[ii][2*jj+1]=-1.0;
    }
  }

  for (int ii=0; ii<TEST3_CHAN_OUT; ii++) test3_biases[ii]=ii*0.01;
}


//*******************************************************
// Top Level Function
//*******************************************************

void test_conv(
      hls::stream<data_t>    &data_i,
      hls::stream<data_t>    &data_q,
      hls::stream<result_t>  &result)
{
  // Remove ap ctrl ports (ap_start, ap_ready, ap_idle, etc) since we only use the AXI-Stream ports
  #pragma HLS INTERFACE ap_ctrl_none port=return


  // TEST 1
  // nnet::conv_iq<data_t, result_t, weight_t, bias_t, accum_t, TEST1_N_IN, TEST1_N_FILT, TEST1_CHAN_OUT>(data_i, data_q, result, test1_weights, test1_biases);


  //TEST 2
  // nnet::conv_iq<data_t, result_t, weight_t, bias_t, accum_t, TEST2_N_IN, TEST2_N_FILT, TEST2_CHAN_OUT>(data_i, data_q, result, test2_weights, test2_biases);

  // TEST 3
  weight_t test3_weights[TEST3_N_FILT][2*TEST3_CHAN_OUT];
  bias_t test3_biases[TEST3_CHAN_OUT];
  get_test3(test3_weights, test3_biases);
  nnet::conv_iq<data_t, result_t, weight_t, bias_t, accum_t, TEST3_N_IN, TEST3_N_FILT, TEST3_CHAN_OUT>(data_i, data_q, result, test3_weights, test3_biases);
}
