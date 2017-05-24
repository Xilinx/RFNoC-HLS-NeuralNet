#ifndef NNET_CONV_H_
#define NNET_CONV_H_

#include "nnet_default.h"
#include "hls_stream.h"

namespace nnet {

template<class data_T, class res_T, class weight_T, class bias_T, class acc_T, int Y_IN, int Y_FILT, int CHAN_OUT>
void conv_iq(
    hls::stream<data_T> &data_i,
    hls::stream<data_T> &data_q,
    hls::stream<res_T>  &res,
    weight_T  weights[Y_FILT][2][CHAN_OUT],
    bias_T    biases[CHAN_OUT])
{
    // Initial directives used from HLS User guide, pg 381
    // (https://www.xilinx.com/support/documentation/sw_manuals/xilinx2015_4/ug902-vivado-high-level-synthesis.pdf)

    #pragma HLS DATAFLOW
    #pragma HLS ARRAY_PARTITION variable=buffer complete

    data_T buffer[Y_FILT][2];

    // Horizontal convolution
    RowLoop:for(int row = 0; row < Y_IN; row++) {
        data_T i_val = data_i.read();
        data_T q_val = data_q.read();
        std::cout << "Read " << i_val << " + " << q_val << "j" << std::endl;
        acc_T i_out = 0;
        acc_T q_out = 0;
        OutFiltLoop:for(int ii = 0; ii < Y_FILT; ii++){
            OutChanLoop:for(int jj = 0; jj < CHAN_OUT; jj++) {
                if (jj==0) {
                    // Shift operation for buffer
                    buffer[ii][0] = ii < Y_FILT - 1 ? buffer[ii + 1][0] : i_val;
                    buffer[ii][1] = ii < Y_FILT - 1 ? buffer[ii + 1][1] : q_val;
                }
                i_out += buffer[ii][0] * weights[ii][0][jj];
                q_out += buffer[ii][1] * weights[ii][1][jj];
                // std::cout << "\tBuffr: " << buffer[ii][0] << " + " << buffer[ii][1] << "j" << std::endl;
                // std::cout << "\tWeigt: " << weights[ii][0][jj] << " + " << weights[ii][1][jj] << "j" << std::endl;
                std::cout << "\tAccum: " << i_out << " + " << q_out << "j" << std::endl;
                if (ii==Y_FILT-1 && row >= Y_FILT-1) {
                    // When we hit the last filter sample, add bias term and output
                    res << i_out + q_out + biases[jj];
                    std::cout << "\tResult: " << i_out + q_out + biases[jj] << std::endl;
                }
            }
        }
    }
}

}

#endif
