//
//    nnet_lib: Vivado HLS code for neural-net building blocks
//
//    Copyright (C) 2017 EJ Kreinar
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

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
    // Limitation of conv_iq:
    // 1. One channel input
    // 2. Column only convolution.. no extra "rows"

    // Initial directives used from HLS User guide, pg 381
    // (https://www.xilinx.com/support/documentation/sw_manuals/xilinx2015_4/ug902-vivado-high-level-synthesis.pdf)

    // #pragma HLS DATAFLOW
    data_T buffer[Y_FILT][2];
    acc_T i_out[CHAN_OUT];
    acc_T q_out[CHAN_OUT];

    #pragma HLS ARRAY_PARTITION variable=buffer complete
    #pragma HLS ARRAY_PARTITION variable=i_out complete
    #pragma HLS ARRAY_PARTITION variable=q_out complete
    #pragma HLS ARRAY_PARTITION variable=weights complete dim=3

    // NOTE: Currently we only output data after the kernel is full
    //         (ie: row >= Y_FILT-1)
    // TODO: Find out what states get saved between runs!

    RowLoop:for(int row = 0; row < Y_IN; row++) {
        data_T i_val = data_i.read();
        data_T q_val = data_q.read();
        // std::cout << "Read " << i_val << " + " << q_val << "j" << std::endl;

        BuffLoop:for(int ii = 0; ii < Y_FILT; ii++) {
        #pragma HLS unroll
            // Shift operation for buffer
            buffer[ii][0] = ii < Y_FILT - 1 ? buffer[ii + 1][0] : i_val;
            buffer[ii][1] = ii < Y_FILT - 1 ? buffer[ii + 1][1] : q_val;
        }

        AccumReset:for(int jj = 0; jj < CHAN_OUT; jj++) {
        #pragma HLS unroll
            i_out[jj] = 0;
            q_out[jj] = 0;
        }

        FiltLoop:for(int ii = 0; ii < Y_FILT; ii++){
            ChanLoop:for(int jj = 0; jj < CHAN_OUT; jj++) {
            #pragma HLS unroll
                i_out[jj] += buffer[ii][0] * weights[ii][0][jj];
                q_out[jj] += buffer[ii][1] * weights[ii][1][jj];
                // std::cout << "\tBuffr: " << buffer[ii][0] << " + " << buffer[ii][1] << "j" << std::endl;
                // std::cout << "\tWeigt: " << weights[ii][0][jj] << " + " << weights[ii][1][jj] << "j" << std::endl;
                // std::cout << "\tFiler/Chan: " << ii << "/" << jj << ", Accum: " << i_out[jj] << " + " << q_out[jj] << "j" << std::endl;
            }
        }

        OutputLoop:for(int jj = 0; jj < CHAN_OUT; jj++){
        #pragma HLS pipeline
            // When we hit the last filter sample, add bias term and output
            if (row >= Y_FILT-1) {
                res << i_out[jj] + q_out[jj] + biases[jj];
                // std::cout << "\tResult: " << i_out[jj] + q_out[jj] + biases[jj] << std::endl;
            }
        }
    }
}


template<class data_T, class res_T, class weight_T, class bias_T, class acc_T, int Y_IN, int Y_FILT, int CHAN_OUT>
void conv_iq_manychan(
    hls::stream<data_T> &data_i,
    hls::stream<data_T> &data_q,
    hls::stream<res_T>  &res,
    weight_T  weights[Y_FILT][2][CHAN_OUT],
    bias_T    biases[CHAN_OUT])
{
    // Provide a new conv_iq directive with unrolled channels
    // Use this function to limit number of DSPs at the expense of a bit more throughput and fabric

    // Initial directives used from HLS User guide, pg 381
    // (https://www.xilinx.com/support/documentation/sw_manuals/xilinx2015_4/ug902-vivado-high-level-synthesis.pdf)

    // #pragma HLS DATAFLOW
    data_T buffer[Y_FILT][2];
    acc_T i_out[CHAN_OUT];
    acc_T q_out[CHAN_OUT];

    #pragma HLS ARRAY_PARTITION variable=buffer complete
    #pragma HLS ARRAY_PARTITION variable=i_out complete
    #pragma HLS ARRAY_PARTITION variable=q_out complete
    #pragma HLS ARRAY_PARTITION variable=weights complete dim=3

    // NOTE: Currently we only output data after the kernel is full
    //         (ie: row >= Y_FILT-1)
    // TODO: Find out what states get saved between runs!

    RowLoop:for(int row = 0; row < Y_IN; row++) {
        data_T i_val = data_i.read();
        data_T q_val = data_q.read();
        // std::cout << "Read " << i_val << " + " << q_val << "j" << std::endl;

        BuffLoop:for(int ii = 0; ii < Y_FILT; ii++) {
        #pragma HLS unroll
            // Shift operation for buffer
            buffer[ii][0] = ii < Y_FILT - 1 ? buffer[ii + 1][0] : i_val;
            buffer[ii][1] = ii < Y_FILT - 1 ? buffer[ii + 1][1] : q_val;
        }

        AccumReset:for(int jj = 0; jj < CHAN_OUT; jj++) {
        #pragma HLS unroll
            i_out[jj] = 0;
            q_out[jj] = 0;
        }

        FiltLoop:for(int ii = 0; ii < Y_FILT; ii++){
            ChanLoop:for(int jj = 0; jj < CHAN_OUT; jj++) {
            #pragma HLS unroll factor=32
                i_out[jj] += buffer[ii][0] * weights[ii][0][jj];
                q_out[jj] += buffer[ii][1] * weights[ii][1][jj];
                // std::cout << "\tBuffr: " << buffer[ii][0] << " + " << buffer[ii][1] << "j" << std::endl;
                // std::cout << "\tWeigt: " << weights[ii][0][jj] << " + " << weights[ii][1][jj] << "j" << std::endl;
                // std::cout << "\tFiler/Chan: " << ii << "/" << jj << ", Accum: " << i_out[jj] << " + " << q_out[jj] << "j" << std::endl;
            }
        }

        OutputLoop:for(int jj = 0; jj < CHAN_OUT; jj++){
        #pragma HLS pipeline
            // When we hit the last filter sample, add bias term and output
            if (row >= Y_FILT-1) {
                res << i_out[jj] + q_out[jj] + biases[jj];
                // std::cout << "\tResult: " << i_out[jj] + q_out[jj] + biases[jj] << std::endl;
            }
        }
    }
}


template<class data_T, class res_T, class weight_T, class bias_T, class acc_T, int Y_IN, int CHAN_IN, int Y_FILT, int CHAN_OUT>
void dummy_test(
    hls::stream<data_T> &data, 
    hls::stream<res_T>  &res, 
    weight_T weights[Y_FILT][CHAN_IN][CHAN_OUT],
    bias_T biases[CHAN_IN])
{
    res << data.read();
}


template<class data_T, class res_T, class weight_T, class bias_T, class acc_T, int Y_IN, int CHAN_IN, int Y_FILT>
void conv_1d(
    hls::stream<data_T> &data,
    hls::stream<res_T>  &res,
    weight_T  weights[Y_FILT][CHAN_IN],
    bias_T    biases[CHAN_IN])
{
    // conv_1d: 1-dimensional convolution
    //   - Also includes multiple input channels
    //   - Only allows new data on each ROW
    // Constrain only ONE output channel per input channel

    // Initial directives used from HLS User guide, pg 381
    // (https://www.xilinx.com/support/documentation/sw_manuals/xilinx2015_4/ug902-vivado-high-level-synthesis.pdf)

    #pragma HLS DATAFLOW
    data_T buffer[Y_FILT][CHAN_IN];
    acc_T int_accum;

    #pragma HLS ARRAY_PARTITION variable=buffer complete
    // #pragma HLS ARRAY_PARTITION variable=int_accum complete
    #pragma HLS ARRAY_PARTITION variable=weights complete dim=2

    // NOTE: Currently we only output data after the kernel is full
    //         (ie: row >= Y_FILT-1)
    // TODO: Find out what states get saved between runs!

    RowLoop:for(int row = 0; row < Y_IN; row++) {
        ChanInLoop:for(int chan_in = 0; chan_in < CHAN_IN; chan_in++){
            data_T val = data.read();
            // std::cout << "Read " << val << std::endl;

            BuffLoop:for(int ii = 0; ii < Y_FILT; ii++) {
            #pragma HLS UNROLL
                // Shift operation for buffer
                buffer[ii][chan_in] = ii < Y_FILT - 1 ? buffer[ii + 1][chan_in] : val;
            }

            int_accum = 0;

            FiltLoop:for(int ii = 0; ii < Y_FILT; ii++){
            #pragma HLS UNROLL
                int_accum += buffer[ii][chan_in] * weights[ii][chan_in];
                // std::cout << "\tFiler/ChIn/ChOut: " << ii << "/" << chan_in << "/" << jj << ", Buffer: " << buffer[ii][chan_in] << std::endl;
                // std::cout << "\tAccum: " << int_accum[jj] << std::endl;
                // std::cout << "\tWeight: " << weights[ii][chan_in][jj] << std::endl;
            }

            // When we hit the last filter sample, add bias term and output
            if (row >= Y_FILT-1) {
                res << int_accum + biases[chan_in];
                // std::cout << "\tResult: " << int_accum[jj] + biases[chan_in][jj]] << std::endl;
            }
        }
    }
}

}

#endif
