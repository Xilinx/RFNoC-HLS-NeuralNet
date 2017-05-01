#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "nnet_basic.h"
#include "nnet_helpers.h"

// int main(int argc, char **argv)
// {
//
//   // Dummy data for now
//   input_t data[N_LAYER_IN] = {0, 1, 1};
//   coeff_t coeffs[N_LAYER_IN][N_LAYER_OUT] = {{1,  0,  0},
//                                              {0,  2,  0},
//                                              {0, 10, 20}};
//   bias_t  biases[N_LAYER_OUT] = {1, 2, 3};
//   short int answer[N_LAYER_OUT] = {1, 14, 23};
//
//   // Run the basic neural net block
//   result_t res[N_LAYER_OUT];
//   nnet_basic(data, coeffs, biases, res);
//
//   // Print result vector
//   int err_cnt = 0;
//   for (int ii = 0; ii < N_LAYER_OUT; ii++) {
//     std::cout << " Expected: " << answer[ii] << "   Received: " << res[ii] << std::endl;
//     if (res[ii] != answer[ii]) err_cnt++;
//   }
//   return err_cnt;
// }

int main(int argc, char **argv)
{
  // DATA FROM UDACITY TENSORFLOW CLASS
  // 1-Layer test

  input_t  data[N_LAYER_IN];
  coeff_t coeffs[N_LAYER_IN][N_LAYER_OUT];
  bias_t  biases[N_LAYER_OUT];
  float answer[N_LAYER_OUT];

  // Load data from file
  int rval = 0;
  rval = read_file_1D<input_t>("data/mnist_validation_data_784x1.dat", data, N_LAYER_IN);
  rval = read_file_2D<coeff_t>("data/mnist_layer1_weights_784x10.dat", coeffs[0], N_LAYER_IN, N_LAYER_OUT);
  rval = read_file_1D<bias_t>("data/mnist_layer1_biases_10x1.dat", biases, N_LAYER_OUT);
  rval = read_file_1D<float>("data/mnist_validation_output_10x1.dat", answer, N_LAYER_OUT);

  // Run the basic neural net block
  result_t res[N_LAYER_OUT];
  nnet_basic(data, coeffs, biases, res);

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

