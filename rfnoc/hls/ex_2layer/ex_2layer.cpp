#include "nnet_layer.h"
#include "nnet_activation.h"

#include "ex_2layer.h"

#include "data/mnist_layer1_weights_small_784x256.h"
#include "data/mnist_layer1_biases_small_256x1.h"
#include "data/mnist_layer2_weights_small_256x10.h"
#include "data/mnist_layer2_biases_small_10x1.h"

// AXI-Stream port type is compatible with pointer, reference, & array input / ouputs only
// See UG902 Vivado High Level Synthesis guide (2014.4) pg 157 Figure 1-49
void ex_2layer(
      hls::stream<input_t> &data,
      hls::stream<result_t> &res,
      unsigned short &const_size_in,
      unsigned short &const_size_out)
{
    // Remove ap ctrl ports (ap_start, ap_ready, ap_idle, etc) since we only use the AXI-Stream ports
    #pragma HLS INTERFACE ap_ctrl_none port=return

    #pragma HLS DATAFLOW

    // Connect size indicators
    #pragma HLS INTERFACE ap_none port=const_size_in
    #pragma HLS INTERFACE ap_none port=const_size_out
    const_size_in   = N_LAYER_IN;
    const_size_out  = N_LAYER_OUT;

    // ****************************************
    // NETWORK INSTATIATION
    // ****************************************

    // LAYER 1
    hls::stream<layer1_t> logits1, hidden1;
    nnet::compute_layer<input_t, layer1_t, weight_t, bias_t, accum_t, N_LAYER_IN, N_LAYER_1>(data, logits1, weights1, biases1);
    nnet::relu<layer1_t, layer1_t, N_LAYER_1>(logits1, hidden1);

    // LAYER 2
    nnet::compute_layer<layer1_t, layer2_t, weight_t, bias_t, accum_t, N_LAYER_1, N_LAYER_2>(hidden1, res, weights2, biases2);
}
