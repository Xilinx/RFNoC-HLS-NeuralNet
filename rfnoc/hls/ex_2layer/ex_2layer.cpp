
#include "nnet_layer.h"
#include "nnet_activation.h"

#include "ex_2layer.h"

// AXI-Stream port type is compatible with pointer, reference, & array input / ouputs only
// See UG902 Vivado High Level Synthesis guide (2014.4) pg 157 Figure 1-49
//void nnet_basic (axis &in, axis &out) {
void ex_2layer(
	  input_t   data[N_LAYER_IN],
	  coeff_t   weights1[N_LAYER_IN][N_LAYER1],
	  bias_t    biases1[N_LAYER1],
	  coeff_t   weights2[N_LAYER1][N_LAYER_OUT],
	  bias_t    biases2[N_LAYER_OUT],
	  result_t  res[N_LAYER_OUT])
{
    // Remove ap ctrl ports (ap_start, ap_ready, ap_idle, etc) since we only use the AXI-Stream ports
//    #pragma HLS INTERFACE ap_ctrl_none port=return

	#pragma HLS DATAFLOW

	result_t logits1[N_LAYER1];
	result_t hidden1[N_LAYER1];

	// LAYER 1
	nnet::layer<input_t, layer1_t, coeff_t, bias_t, accum_t> layer1;
	layer1.compute<N_LAYER_IN, N_LAYER1>(data, logits1, weights1, biases1);
	nnet::relu<layer1_t, layer1_t, N_LAYER1>(logits1, hidden1);

	// LAYER 2
	nnet::layer<layer1_t, result_t, coeff_t, bias_t, accum_t> layer2;
	layer2.compute<N_LAYER1, N_LAYER_OUT>(hidden1, res, weights2, biases2);
}
