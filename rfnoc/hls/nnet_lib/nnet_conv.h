#ifndef NNET_CONV_H_
#define NNET_CONV_H_

#include "nnet_default.h"

namespace nnet {

template<class data_T, class res_T, class weight_T, class bias_T, class acc_T>
class conv {
    protected:
    private:
    public:
        conv() { /*Do Nothing (for now)*/};

        template<int N_IN, int N_OUT>
        void compute(data_T data[N_IN], res_T res[N_OUT], weight_T weights[N_IN][N_OUT], bias_T biases[N_OUT]);
};

template<class data_T, class res_T, class weight_T, class bias_T, class acc_T>
template<int X_IN, int Y_IN, int CHAN_IN, int X_FILT, int Y_FILT, int CHAN_OUT>
void layer<data_T, res_T, weight_T, bias_T, acc_T>::conv(
    data_T    data[X_IN][Y_IN][CHAN_IN],
    res_T     res[X_IN][Y_IN][CHAN_OUT],
    weight_T  weights[X_FILT][Y_FILT][CHAN_IN][CHAN_OUT],
    bias_T    biases[CHAN_OUT])
{
    // Initial directives used from HLS User guide, pg 381
    // (https://www.xilinx.com/support/documentation/sw_manuals/xilinx2015_4/ug902-vivado-high-level-synthesis.pdf)

    #pragma HLS DATAFLOW
    #pragma HLS ARRAY_PARTITION variable=linebuf dim=1 complete

    hls::stream<T> hconv("hconv");
    hls::stream<T> vconv("vconv");

    // Horizontal convolution
    HConvH:for(int col = 0; col < height; col++) {
        HConvW:for(int row = 0; row < width; row++) {
        #pragma HLS PIPELINE
            HConv:for(int i = 0; i < K; i++) {
            }
        }
    }

    // Vertical convolution
    VConvH:for(int col = 0; col < height; col++) {
        VConvW:for(int row = 0; row < vconv_xlim; row++) {
        #pragma HLS PIPELINE
        #pragma HLS DEPENDENCE variable=linebuf inter false
            VConv:for(int i = 0; i < K; i++) {
            }
    }

    // Border:for (int i = 0; i < height; i++) {
    //     for (int j = 0; j < width; j++) {
    //     #pragma HLS PIPELINE
    //     }
    // }
}

}

#endif
