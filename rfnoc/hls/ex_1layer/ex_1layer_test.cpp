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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ex_1layer.h"
#include "nnet_helpers.h"

int main(int argc, char **argv)
{
  // DATA FROM UDACITY TENSORFLOW CLASS
  // 1-Layer test

  input_t  data[N_LAYER_IN];
  coeff_t weights[N_LAYER_IN][N_LAYER_OUT];
  bias_t  biases[N_LAYER_OUT];
  float answer[N_LAYER_OUT];

  // Load data from file
  int rval = 0;
  rval = nnet::read_file_1D<input_t, N_LAYER_IN>("data/mnist_validation_data_784x1.dat", data);
  rval = nnet::read_file_1D<float, N_LAYER_OUT>("data/mnist_validation_output_10x1.dat", answer);

  // Run the basic neural net block
  unsigned short size_in, size_out;

  hls::stream<input_t> data_str;
  for (int idat=0; idat < N_LAYER_IN; idat++) {
    data_str << data[idat];
  }

  hls::stream<result_t> res_str;
  ex_1layer(data_str, res_str, size_in, size_out);

  std::cout << "Found network size: " << size_in << "x" << size_out << std::endl;

  // Print result vector
  int err_cnt = 0;
  float err, curr_data;
  for (int ii = 0; ii < N_LAYER_OUT; ii++) {
    curr_data = res_str.read();
    err = curr_data-answer[ii];
    std::cout << " Expected: " << answer[ii] << "   Received: " << curr_data << "  ErrVal: " << err << std::endl;
    if (abs(err) > 0.5) err_cnt++;
  }
  std::cout<< err_cnt << std::endl;
  return err_cnt;
}

