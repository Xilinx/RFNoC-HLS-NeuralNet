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
#include <iostream>

#include "parameters.h"
#include "keras_3layer.h"

#include "nnet_layer.h"
#include "nnet_conv.h"
#include "nnet_conv2d.h"
#include "nnet_batchnorm.h"
#include "nnet_activation.h"

//hls-fpga-machine-learning insert weights
#include "weights/w1.h"
#include "weights/b1.h"
#include "weights/w2.h"
#include "weights/b2.h"
#include "weights/w3.h"
#include "weights/b3.h"
#include "weights/w4.h"
#include "weights/b4.h"

void keras_3layer(
		  input_t data[N_INPUTS],
		  result_t res[N_OUTPUTS],
		  unsigned short &const_size_in,
		  unsigned short &const_size_out)
{

    //hls-fpga-machine-learning insert IO
    #pragma HLS INTERFACE axis port=data,res 
    #pragma HLS DATAFLOW 


    const_size_in   = N_INPUTS;
    const_size_out  = N_OUTPUTS;

    // ****************************************
    // NETWORK INSTANTIATION
    // ****************************************

    //hls-fpga-machine-learning insert layers

    layer1_t layer1_out[N_LAYER_1];
    #pragma HLS STREAM variable=layer1_out depth=1
    layer1_t logits1[N_LAYER_1];
    #pragma HLS STREAM variable=logits1 depth=1
    nnet::compute_layer<input_t, layer1_t, config1>(data, logits1, w1, b1);
    nnet::relu<layer1_t, layer1_t, relu_config1>(logits1, layer1_out);

    layer2_t layer2_out[N_LAYER_2];
    #pragma HLS STREAM variable=layer2_out depth=1
    layer2_t logits2[N_LAYER_2];
    #pragma HLS STREAM variable=logits2 depth=1
    nnet::compute_layer<layer1_t, layer2_t, config2>(layer1_out, logits2, w2, b2);
    nnet::relu<layer2_t, layer2_t, relu_config2>(logits2, layer2_out);

    layer3_t layer3_out[N_LAYER_3];
    #pragma HLS STREAM variable=layer3_out depth=1
    layer3_t logits3[N_LAYER_3];
    #pragma HLS STREAM variable=logits3 depth=1
    nnet::compute_layer<layer2_t, layer3_t, config3>(layer2_out, logits3, w3, b3);
    nnet::relu<layer3_t, layer3_t, relu_config3>(logits3, layer3_out);

    result_t logits4[N_OUTPUTS];
    #pragma HLS STREAM variable=logits4 depth=1
    nnet::compute_layer<layer3_t, result_t, config4>(layer3_out, logits4, w4, b4);
    nnet::softmax<result_t, result_t, softmax_config4>(logits4, res);


}
