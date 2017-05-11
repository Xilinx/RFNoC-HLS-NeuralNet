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
  rval = read_file_1D<input_t, N_LAYER_IN>("data/mnist_validation_data_784x1.dat", data);
    rval = read_file_1D<float, N_LAYER_OUT>("data/mnist_validation_output_10x1.dat", answer);
//  rval = read_file_2D<coeff_t, N_LAYER_IN, N_LAYER_OUT>("data/mnist_layer1_weights_784x10.dat", weights);
//  rval = read_file_1D<bias_t, N_LAYER_OUT>("data/mnist_layer1_biases_10x1.dat", biases);
//  rval = read_file_1D<input_t, N_LAYER_IN>("data/validation_data_784x1.dat", data);
//  rval = read_file_2D<coeff_t, N_LAYER_IN, N_LAYER_OUT>("data/mnist_layer1_weights_small_784x256.dat", weights);
//  rval = read_file_1D<bias_t, N_LAYER_OUT>("data/mnist_layer1_biases_small_256x1.dat", biases);
//  rval = read_file_1D<float, N_LAYER_OUT>("data/validation_layer1_small_256x1.dat", answer);

  // Run the basic neural net block
  result_t res[N_LAYER_OUT];
  ex_1layer(data, res);

  // Print result vector
  int err_cnt = 0;
  float err;
  for (int ii = 0; ii < N_LAYER_OUT; ii++) {
	err = float(res[ii])-answer[ii];
    std::cout << " Expected: " << answer[ii] << "   Received: " << res[ii] << "  ErrVal: " << err << std::endl;
    if (abs(err) > 0.5) err_cnt++;
  }
  std::cout<< err_cnt << std::endl;
  return err_cnt;
}

