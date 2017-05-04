#ifndef NNET_LAYER_H_
#define NNET_LAYER_H_

#include "nnet_default.h"

template<class data_T, class res_T, class weight_T, class bias_T, class acc_T>
class nnet_layer {
    protected:
        template<int N_IN, int N_OUT>
        void compute_small_outputs(data_T data[N_IN], res_T res[N_OUT], weight_T weights[N_IN][N_OUT], bias_T biases[N_OUT]);

        template<int N_IN, int N_OUT>
        void compute_medium_outputs(data_T data[N_IN], res_T res[N_OUT], weight_T weights[N_IN][N_OUT], bias_T biases[N_OUT]);

        template<int N_IN, int N_OUT>
        void compute_large_outputs(data_T data[N_IN], res_T res[N_OUT], weight_T weights[N_IN][N_OUT], bias_T biases[N_OUT]);
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
    if (N_OUT >= 512) {
        compute_large_outputs<N_IN, N_OUT>(data, res, weights, biases);
    }
    else if (N_OUT >= 32) {
        compute_medium_outputs<N_IN, N_OUT>(data, res, weights, biases);
    }
    else {
        compute_small_outputs<N_IN, N_OUT>(data, res, weights, biases);
    }
}

// *************************************************
//       Possible implementation options
// *************************************************

template<class data_T, class res_T, class weight_T, class bias_T, class acc_T>
template<int N_IN, int N_OUT>
void nnet_layer<data_T, res_T, weight_T, bias_T, acc_T>::compute_small_outputs(
    data_T    data[N_IN],
    res_T     res[N_OUT],
    weight_T  weights[N_IN][N_OUT],
    bias_T    biases[N_OUT])
{
    data_T data_cache;
    acc_T acc[N_OUT];

    #pragma HLS INTERFACE ap_fifo port=data
    #pragma HLS INTERFACE ap_fifo port=res
	#pragma HLS ARRAY_RESHAPE variable=weights complete dim=2
    #pragma HLS ARRAY_PARTITION variable=acc complete dim=1

    Reset: for(int iacc = 0; iacc < N_OUT; iacc++)
    #pragma HLS UNROLL
        acc[iacc] = 0;

    NewInput: for(int ii = 0; ii < N_IN; ii++) {
    #pragma HLS PIPELINE

        Product: for(int jj = 0; jj < N_OUT; jj++) {
        #pragma HLS UNROLL
            if (jj == 0) data_cache = data[ii];
            weight_T weight = weights[ii][jj];
            acc[jj] += data_cache * weight;
            //std::cout << "Multiplying: " << weight << " x " << data_cache[ii] << std::endl;
        }
    }

    Result: for(int ires = 0; ires < N_OUT; ires++)
	#pragma HLS PIPELINE
        res[ires] = acc[ires] + biases[ires];
}

template<class data_T, class res_T, class weight_T, class bias_T, class acc_T>
template<int N_IN, int N_OUT>
void nnet_layer<data_T, res_T, weight_T, bias_T, acc_T>::compute_medium_outputs(
    data_T    data[N_IN],
    res_T     res[N_OUT],
    weight_T  weights[N_IN][N_OUT],
    bias_T    biases[N_OUT])
{
    data_T data_cache;
    acc_T acc[N_OUT];

    #pragma HLS INTERFACE ap_fifo port=data
    #pragma HLS INTERFACE ap_fifo port=res
    #pragma HLS ARRAY_RESHAPE variable=weights block factor=32 dim=2
    #pragma HLS ARRAY_PARTITION variable=acc block factor=32
	#pragma HLS allocation instances=mul limit=32 operation

    Reset: for(int iacc = 0; iacc < N_OUT; iacc++)
    #pragma HLS UNROLL
        acc[iacc] = 0;

    NewInput: for(int ii = 0; ii < N_IN; ii++) {
    #pragma HLS PIPELINE

        Product: for(int jj = 0; jj < N_OUT; jj++) {
        #pragma HLS UNROLL factor=32
//        	if (ii == 0) acc[jj] = 0;
            if (jj == 0) data_cache = data[ii];
            weight_T weight = weights[ii][jj];
            acc[jj] += data_cache * weight;
        }
    }

    Result: for(int ires = 0; ires < N_OUT; ires++)
	#pragma HLS PIPELINE
        res[ires] = acc[ires] + biases[ires];
}

template<class data_T, class res_T, class weight_T, class bias_T, class acc_T>
template<int N_IN, int N_OUT>
void nnet_layer<data_T, res_T, weight_T, bias_T, acc_T>::compute_large_outputs(
    data_T    data[N_IN],
    res_T     res[N_OUT],
    weight_T  weights[N_IN][N_OUT],
    bias_T    biases[N_OUT])
{
    data_T data_cache;
    acc_T acc[N_OUT];

    #pragma HLS INTERFACE ap_fifo port=data
    #pragma HLS INTERFACE ap_fifo port=res
    #pragma HLS ARRAY_RESHAPE variable=weights block factor=256 dim=2
    #pragma HLS ARRAY_PARTITION variable=acc block factor=256
	#pragma HLS allocation instances=mul limit=256 operation

    Reset: for(int iacc = 0; iacc < N_OUT; iacc++)
    #pragma HLS UNROLL
        acc[iacc] = 0;

    NewInput: for(int ii = 0; ii < N_IN; ii++) {
    #pragma HLS PIPELINE

        Product: for(int jj = 0; jj < N_OUT; jj++) {
        #pragma HLS UNROLL factor=256
//        	if (ii == 0) acc[jj] = 0;
            if (jj == 0) data_cache = data[ii];
            weight_T weight = weights[ii][jj];
            acc[jj] += data_cache * weight;
        }
    }

    Result: for(int ires = 0; ires < N_OUT; ires++)
	#pragma HLS PIPELINE
        res[ires] = acc[ires] + biases[ires];
}


#endif
