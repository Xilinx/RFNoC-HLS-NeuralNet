#ifndef NNET_ACTIVATION_H_
#define NNET_ACTIVATION_H_

#include <math.h>
#include "ap_fixed.h"

namespace nnet {

template<class data_T, class res_T, int N_IN>
void  relu(data_T data[N_IN], res_T res[N_IN])
{
    #pragma HLS INTERFACE ap_fifo port=data
    #pragma HLS INTERFACE ap_fifo port=res
    data_T datareg;
    for (int ii=0; ii<N_IN; ii++) {
        datareg = data[ii];
        if (datareg > 0) res[ii] = datareg;
        else res[ii] = 0;
    }
}

template<class data_T, class N_TABLE>
void init_sigmoid_table(data_T table_out[N_IN])
{
    // "Default" sigmoid function:
    //   result = 1/(1+e^(-x))
    for (int ii = 0; ii < N_IN; ii++) {
        float in_val = 8*(ii-N_IN/2)/N_IN;
        data_T real_val = 1.0 / (1 + exp(-in_val));
        table_out[ii] = real_val;
    }
}

template<class data_T, class res_T, int N_IN>
void  sigmoid(data_T data[N_IN], res_T res[N_IN])
{
    #pragma HLS INTERFACE ap_fifo port=data
    #pragma HLS INTERFACE ap_fifo port=res

    // Initialize the lookup table
    res_T sigmoid_table[256];
    init_sigmoid_table<res_T, 256>(sigmoid_table);

    // Index into the lookup table based on data
    data_T datareg;
    ap_fixed<8,4,AP_RND_ZERO> data_round;
    short index;
    for (int ii=0; ii<N_IN; ii++) {
        data_round = data[ii];
        index = data_round << 4;
        res[ii] = sigmoid_table[index];
    }
}

template<class data_T, class res_T, int N_IN>
void  tanh(data_T data[N_IN], res_T res[N_IN])
{
    // Use a lookup table for the tanh function
    #pragma HLS INTERFACE ap_fifo port=data
    #pragma HLS INTERFACE ap_fifo port=res
    data_T datareg;
    for (int ii=0; ii<N_IN; ii++) {
        datareg = data[ii];
        if (datareg > 0) res[ii] = datareg;
        else res[ii] = 0;
    }
}

}

#endif
