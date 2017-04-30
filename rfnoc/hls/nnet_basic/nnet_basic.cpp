#include "nnet_basic.h"

//coeff_t coeff[N] = {0,-10,-9,23,56,63,56,23,-9,-10,0,};

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

	nnet_layer(data, weights, biases, res);
}

void nnet_layer(
	  input_t   data[N_LAYER_IN],
	  coeff_t   weights[N_LAYER_IN][N_LAYER_OUT],
	  bias_t    biases[N_LAYER_OUT],
	  result_t  res[N_LAYER_OUT])
{
	#pragma HLS INTERFACE ap_fifo port=data
	#pragma HLS ARRAY_RESHAPE variable=weights complete dim=2
	#pragma HLS INTERFACE ap_fifo port=res

  	input_t data_cache;
  	accum_t acc[N_LAYER_OUT];
    #pragma HLS ARRAY_PARTITION variable=acc complete dim=1

	Reset: for(int iacc = 0; iacc < N_LAYER_OUT; iacc++)
	#pragma HLS UNROLL
		acc[iacc] = 0;

  	NewInput: for(int ii = 0; ii < N_LAYER_IN; ii++) {
	#pragma HLS PIPELINE

		Product: for(int jj = 0; jj < N_LAYER_OUT; jj++) {
		#pragma HLS UNROLL
			if (jj == 0) data_cache = data[ii];
			coeff_t weight = weights[ii][jj];
			acc[jj] += data_cache * weight;
			//std::cout << "Multiplying: " << weight << " x " << data_cache[ii] << std::endl;
		}
    }

	Result: for(int ires = 0; ires < N_LAYER_OUT; ires++)
		res[ires] = acc[ires] + biases[ires];
}

