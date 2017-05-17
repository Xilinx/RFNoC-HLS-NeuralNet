#include "ex_1layer.h"
#include "nnet_layer.h"

//#include "data/mnist_layer1_biases_small_256x1.h"
//#include "data/mnist_layer1_weights_small_784x256.h"
#include "data/mnist_layer1_biases_10x1.h"
#include "data/mnist_layer1_weights_784x10.h"

// AXI-Stream port type is compatible with pointer, reference, & array input / ouputs only
// See UG902 Vivado High Level Synthesis guide (2014.4) pg 157 Figure 1-49
//void nnet_basic (axis &in, axis &out) {
void ex_1layer(
	  input_t   data[N_LAYER_IN],
	  result_t  res[N_LAYER_OUT],
	  unsigned short &const_size_in,
	  unsigned short &const_size_out)
{
    // Remove ap ctrl ports (ap_start, ap_ready, ap_idle, etc) since we only use the AXI-Stream ports
    #pragma HLS INTERFACE ap_ctrl_none port=return

	// Connect size indicators
    #pragma HLS INTERFACE ap_none port=const_size_in
    #pragma HLS INTERFACE ap_none port=const_size_out
	const_size_in   = N_LAYER_IN;
	const_size_out  = N_LAYER_OUT;

	nnet::layer<input_t, result_t, coeff_t, bias_t, accum_t> layer1;
	layer1.compute<N_LAYER_IN, N_LAYER_OUT>(data, res, weights, biases);
}
