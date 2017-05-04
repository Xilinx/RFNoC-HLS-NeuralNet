#ifndef NNET_ACTIVATION_H_
#define NNET_ACTIVATION_H_

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

}

#endif
