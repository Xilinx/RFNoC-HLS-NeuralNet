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

#ifndef TEST_CONV1D_H_
#define TEST_CONV1D_H_

#include <complex>
#include "ap_fixed.h"
#include "hls_stream.h"

typedef ap_fixed<16,3>  data_t;
typedef ap_fixed<16,5>  result_t;
typedef ap_fixed<16,3>  weight_t;
typedef ap_fixed<16,3>  bias_t;
typedef ap_fixed<32,10>  accum_t;

// typedef float  data_t;
// typedef float  result_t;
// typedef float  weight_t;
// typedef float  bias_t;
// typedef float accum_t;

#define TEST1_N_IN        10
#define TEST1_CHAN        2
#define TEST1_N_FILT      4
#define TEST1_N_OUT       TEST1_N_IN-TEST1_N_FILT+1

#define TEST2_N_IN        60
#define TEST2_CHAN        64
#define TEST2_N_FILT      16
#define TEST2_N_OUT       TEST2_N_IN-TEST2_N_FILT+1


// Prototype of top level function for C-synthesis
void test_conv1d(
    hls::stream<data_t>   &data,
    hls::stream<result_t> &result);


#endif
