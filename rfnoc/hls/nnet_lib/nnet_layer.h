#ifndef NNET_LAYER_H_
#define NNET_LAYER_H_

#include "nnet_default.h"

template<class data_T, class res_T, class weight_T, class bias_T, class acc_T>
class nnet_layer {
    protected:
    private:
    public:
        nnet_layer() { /*Do Nothing (for now)*/};

        template<int N_IN, int N_OUT>
        void compute(data_T data[N_IN], res_T res[N_OUT], weight_T weights[N_IN][N_OUT], bias_T biases[N_OUT]);
};

template<class data_T, class res_T, class weight_T, class bias_T, class acc_T>
template<int N_IN, int N_OUT>
void nnet_layer<data_T, res_T, weight_T, bias_T, acc_T>::compute(
	data_T    data[N_IN],
	res_T     res[N_OUT],
	weight_T  weights[N_IN][N_OUT],
	bias_T    biases[N_OUT])
{
	#pragma HLS INTERFACE ap_fifo port=data
	#pragma HLS ARRAY_RESHAPE variable=weights complete dim=2
	#pragma HLS INTERFACE ap_fifo port=res

	data_T data_cache;
  	acc_T acc[N_OUT];
    #pragma HLS ARRAY_PARTITION variable=acc complete dim=1

	Reset: for(int iacc = 0; iacc < N_OUT; iacc++)
	#pragma HLS UNROLL
		acc[iacc] = 0;

  	NewInput: for(int ii = 0; ii < N_IN; ii++) {
	#pragma HLS PIPELINE

		Product: for(int jj = 0; jj < N_OUT; jj++) {
		#pragma HLS UNROLL
			if (jj == 0) data_cache = data[ii];
			coeff_t weight = weights[ii][jj];
			acc[jj] += data_cache * weight;
			//std::cout << "Multiplying: " << weight << " x " << data_cache[ii] << std::endl;
		}
    }

	Result: for(int ires = 0; ires < N_OUT; ires++)
		res[ires] = acc[ires] + biases[ires];
}

#endif
