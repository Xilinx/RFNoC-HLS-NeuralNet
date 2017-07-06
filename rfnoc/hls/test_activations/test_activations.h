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

#ifndef TEST_ACTIVATIONS_H_
#define TEST_ACTIVATIONS_H_

#include <complex>
#include "ap_fixed.h"
#include "hls_stream.h"

typedef ap_fixed<16,8> data_t;
typedef ap_fixed<16,2> result_t;
//typedef float data_t;
//typedef float result_t;

#define N_LAYER 100

// Prototype of top level function for C-synthesis
void test_activations(
		hls::stream<data_t> &data,
      hls::stream<result_t> &result);

#endif
