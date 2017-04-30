#ifndef NNET_LAYER_H_
#define NNET_LAYER_H_

#ifndef N_LAYER_IN
#define N_LAYER_IN 1000
#endif

#ifndef N_LAYER_OUT
#define N_LAYER_OUT 100
#endif

template<class data_T, class res_T, class weight_T, class bias_T, class acc_T>
void nnet_layer(
	  data_T    data[N_LAYER_IN],
	  res_T     res[N_LAYER_OUT],
	  weight_T  weights[N_LAYER_IN][N_LAYER_OUT],
	  bias_T    biases[N_LAYER_OUT])
{
	#pragma HLS INTERFACE ap_fifo port=data
	#pragma HLS ARRAY_RESHAPE variable=weights complete dim=2
	#pragma HLS INTERFACE ap_fifo port=res

  	input_t data_cache;
  	acc_T acc[N_LAYER_OUT];
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

#endif
