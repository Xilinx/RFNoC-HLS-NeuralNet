#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "nnet_helpers.h"
#include "ex_2layer.h"

int main(int argc, char **argv)
{
  // DATA FROM UDACITY TENSORFLOW CLASS
  // 2-Layer test

  input_t  data[N_LAYER_IN];
  float answer[N_LAYER_OUT];

  // Load data from file
  int rval = 1;
  rval += nnet::read_file_1D<input_t, N_LAYER_IN>("data/validation_data_784x1.dat", data);
  rval += nnet::read_file_1D<float, N_LAYER_OUT>("data/validation_layer2_small_10x1.dat", answer);
  if (rval == 0) {
	  std::cout << "Failed to open files" << std::endl;
	  return -1;
  }

  hls::stream<input_t> data_str;
  for (int idat=0; idat < N_LAYER_IN; idat++) {
    data_str << data[idat];
  }

  // Run the basic neural net block
  hls::stream<result_t> res_str;
  unsigned short size_in, size_out;
  ex_2layer(data_str, res_str, size_in, size_out);

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
