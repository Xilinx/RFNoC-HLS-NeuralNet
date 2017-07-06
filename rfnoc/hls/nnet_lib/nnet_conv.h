//
//    rfnoc-hls-neuralnet: Vivado HLS code for neural-net building blocks
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
    weight_T  weights[Y_FILT][2*CHAN_OUT],
    bias_T    biases[CHAN_OUT])
{
    // Limitation of conv_iq:
    // 1. One channel input
    // 2. Column only convolution.. no extra "rows"

    // Initial directives used from HLS User guide, pg 381
    // (https://www.xilinx.com/support/documentation/sw_manuals/xilinx2015_4/ug902-vivado-high-level-synthesis.pdf)

    data_T buffer[Y_FILT][2];
    acc_T i_out[CHAN_OUT];
    acc_T q_out[CHAN_OUT];

    #pragma HLS ARRAY_PARTITION variable=buffer complete
    #pragma HLS ARRAY_PARTITION variable=i_out complete
    #pragma HLS ARRAY_PARTITION variable=q_out complete
    #pragma HLS ARRAY_PARTITION variable=weights complete dim=2

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
            #pragma HLS PIPELINE
                i_out[jj] += buffer[ii][0] * weights[ii][2*jj];
                q_out[jj] += buffer[ii][1] * weights[ii][2*jj+1];
                // std::cout << "\tBuffr: " << buffer[ii][0] << " + " << buffer[ii][1] << "j" << std::endl;
                // std::cout << "\tWeigt: " << weights[ii][2*jj] << " + " << weights[ii][2*jj+1] << "j" << std::endl;
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
    weight_T  weights[Y_FILT][2*CHAN_OUT],
    bias_T    biases[CHAN_OUT])
{
    // NOTE: Currently NOT functional!! Does not synthesize well.
    // Needs more work on partitioning + code compatibility

    // GOAL:
    // Provide a new conv_iq directive with unrolled channels and RAM weight storage.
    // Use this function to limit number of DSPs, limit fabric, at the expense of throughput and BRAMs
    // Partitions arrays into block RAM

    // Initial directives used from HLS User guide, pg 381
    // (https://www.xilinx.com/support/documentation/sw_manuals/xilinx2015_4/ug902-vivado-high-level-synthesis.pdf)

    data_T buffer[Y_FILT][2];
    data_T temp_i, temp_q;
    acc_T i_out[CHAN_OUT];
    acc_T q_out[CHAN_OUT];

    #pragma HLS ARRAY_PARTITION variable=buffer complete
    #pragma HLS ARRAY_PARTITION variable=i_out cyclic factor=32 dim=1
    #pragma HLS ARRAY_PARTITION variable=q_out cyclic factor=32 dim=1
    #pragma HLS ARRAY_RESHAPE variable=weights cyclic factor=64 dim=2

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
        #pragma HLS unroll factor=16
            i_out[jj] = 0;
            q_out[jj] = 0;
        }

        FiltLoop:for(int ii = 0; ii < Y_FILT; ii++){
            temp_i = buffer[ii][0];
            temp_q = buffer[ii][1];
            ChanLoop:for(int jj = 0; jj < CHAN_OUT; jj++) {
            #pragma HLS unroll factor=16
                i_out[jj] += temp_i * weights[ii][2*jj];
                q_out[jj] += temp_q * weights[ii][2*jj+1];
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


template<class data_T, class res_T, class weight_T, class bias_T, class acc_T, int Y_IN, int Y_FILT, int N_CHAN>
void conv_1d(
    hls::stream<data_T> &data,
    hls::stream<res_T>  &res,
    weight_T  weights[Y_FILT][N_CHAN],
    bias_T    biases[N_CHAN])
{
    // conv_1d: 1-dimensional convolution
    //   - Also includes multiple input channels
    //   - Only allows new data on each ROW (i.e. this is NOT a 2D convolution)
    // Only ONE output channel per input channel

    // Initial directives used from HLS User guide, pg 381
    // (https://www.xilinx.com/support/documentation/sw_manuals/xilinx2015_4/ug902-vivado-high-level-synthesis.pdf)

    // TODO: Figure out how to correctly pipeline FiltLoop-- It sort of needs to be pipelined
    // across iterations of ChanLoop. Otherwise it does not want to consistently hit 10 ns timing

    data_T buffer[Y_FILT][N_CHAN];
    acc_T int_accum[N_CHAN];

    #pragma HLS ARRAY_PARTITION variable=buffer complete
    #pragma HLS ARRAY_PARTITION variable=weights complete

    // NOTE: Currently we only output data after the kernel is full
    //         (ie: row >= Y_FILT-1)
    // TODO: Find out what states get saved between runs!

    RowLoop:for(int row = 0; row < Y_IN; row++) {
        ChanLoop:for(int chan = 0; chan < N_CHAN; chan++){
            data_T val = data.read();
            // std::cout << "Read " << val << std::endl;

            BuffLoop:for(int ii = 0; ii < Y_FILT; ii++) {
            #pragma HLS UNROLL
                // Shift operation for buffer
                buffer[ii][chan] = ii < Y_FILT - 1 ? buffer[ii + 1][chan] : val;
            }

            int_accum[chan] = 0;

            FiltLoop:for(int ii = 0; ii < Y_FILT; ii++){
            #pragma HLS UNROLL factor=4
                int_accum[chan] += buffer[ii][chan] * weights[ii][chan];
                // std::cout << "\tFilter/ChIn: " << ii << "/" << chan << ", Buffer: " << buffer[ii][chan] << std::endl;
                // std::cout << "\tAccum: " << int_accum[chan] << std::endl;
                // std::cout << "\tWeight: " << weights[ii][chan] << std::endl;
            }

            // When we hit the last filter sample, add bias term and output
            if (row >= Y_FILT-1) {
                res << int_accum[chan] + biases[chan];
                // std::cout << "\tResult: " << int_accum[jj] + biases[chan][jj]] << std::endl;
            }
        }
    }
}

}

#endif
