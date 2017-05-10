#include "ex_1layer.h"
#include "nnet_layer.h"

// AXI-Stream port type is compatible with pointer, reference, & array input / ouputs only
// See UG902 Vivado High Level Synthesis guide (2014.4) pg 157 Figure 1-49
//void nnet_basic (axis &in, axis &out) {
void ex_1layer(
	  input_t   data[N_LAYER_IN],
	  coeff_t   weights[N_LAYER_IN][N_LAYER_OUT],
	  bias_t    biases[N_LAYER_OUT],
	  result_t  res[N_LAYER_OUT])
{
    // Remove ap ctrl ports (ap_start, ap_ready, ap_idle, etc) since we only use the AXI-Stream ports
//    #pragma HLS INTERFACE ap_ctrl_none port=return

	nnet::layer<input_t, result_t, coeff_t, bias_t, accum_t> layer1;
	layer1.compute<N_LAYER_IN, N_LAYER_OUT>(data, res, weights, biases);
}
