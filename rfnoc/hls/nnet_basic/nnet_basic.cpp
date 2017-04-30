#include "nnet_basic.h"
#include "nnet_layer.h"

// AXI-Stream port type is compatible with pointer, reference, & array input / ouputs only
// See UG902 Vivado High Level Synthesis guide (2014.4) pg 157 Figure 1-49
//void nnet_basic (axis &in, axis &out) {
void nnet_basic(
	  input_t   data[N_LAYER_IN],
	  coeff_t   weights[N_LAYER_IN][N_LAYER_OUT],
	  bias_t    biases[N_LAYER_OUT],
	  result_t  res[N_LAYER_OUT])
{
    // Remove ap ctrl ports (ap_start, ap_ready, ap_idle, etc) since we only use the AXI-Stream ports
    #pragma HLS INTERFACE ap_ctrl_none port=return
    // Set ports as AXI-Stream
    //#pragma HLS INTERFACE axis port=in
    //#pragma HLS INTERFACE axis port=out
	// Set format of Coeffs:
	// #pragma HLS RESOURCE variable=coeff core=RAM_1P_BRAM
    // Need to pack our complex<short int> into a 32-bit word
    // Otherwise, compiler complains that our AXI-Stream interfaces have two data fields (i.e. data.real, data.imag)
    //#pragma HLS DATA_PACK variable=in.data
    //#pragma HLS DATA_PACK variable=out.data

	// Dummy loopback
	// out.data = in.data;

	nnet_layer<input_t, result_t, coeff_t, bias_t, accum_t>(data, res, weights, biases);
}
