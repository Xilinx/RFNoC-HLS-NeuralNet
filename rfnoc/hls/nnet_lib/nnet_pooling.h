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

#ifndef NNET_POOLING_H_
#define NNET_POOLING_H_

#include <math.h>
#include "ap_fixed.h"
#include "hls_stream.h"

namespace nnet {


// *************************************************
//       Basic Max Pool: 2x pool, 2x stride
// *************************************************
template<class data_T, int N_IN, int N_CHAN>
void  maxpool_2x(hls::stream<data_T> &data, hls::stream<data_T> &pool)
{
    // reduce both the channel and input by 2x 
    // (a constrained example, but that's okay)
    data_T buffer[2][N_CHAN];
    data_T datareg, arg0, arg1, arg2, arg3, temp;
    ap_ufixed<1,1> row_count, chan_count;
    ap_ufixed<16,16> out_count;
    row_count = 0;
    for (int ii=0; ii<N_IN; ii++) {
        row_count = row_count + 1;
        chan_count = 0;
        for (int jj=0; jj<N_CHAN; jj++) {
            chan_count = chan_count + 1;
            buffer[0][jj] = data.read();
            if (ii > 0 && jj > 0 && chan_count == 0 && row_count == 0) {
                // Output the max pool...
                // NOTE: this clearly doesnt scale
                // TODO: Make this scale
                arg0 = buffer[1][jj-1];
                arg1 = buffer[1][jj];
                arg2 = buffer[0][jj-1];
                arg3 = buffer[0][jj];
                if (arg0 > arg1) {
                    if (arg2 > arg3) {
                        if (arg0 > arg2) temp = arg0;
                        else temp = arg2;
                    }
                    else {
                        if (arg0 > arg3) temp = arg0;
                        else temp = arg3;
                    }
                }
                else {
                    if (arg2 > arg3) {
                        if (arg1 > arg2) temp = arg1;
                        else temp = arg2;
                    }
                    else {
                        if (arg1 > arg3) temp = arg1;
                        else temp = arg3;
                    }
                }
                pool << temp;
                //std::cout << "[" << out_count++ << "]" << "[" << ii << "][" << jj << "]: MAX = " << temp << " Vals: " <<  arg0 << ", " << arg1 << ", " << arg2 << ", " << arg3 << std::endl;
            }
        }

        SaveLoop: for (int kk=0; kk<N_CHAN; kk++) {
        #pragma HLS pipeline
            buffer[1][kk] = buffer[0][kk];
        }
    }
}

}

#endif
