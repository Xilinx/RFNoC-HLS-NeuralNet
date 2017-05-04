#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "nnet_helpers.h"
#include "nnet_2layer.h"

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
  coeff_t coeffs1[N_LAYER_IN][N_LAYER1_OUT];
  coeff_t coeffs2[N_LAYER1_OUT][N_LAYER_OUT];
  bias_t  biases1[N_LAYER1_OUT];
  bias_t  biases2[N_LAYER_OUT];
  float answer[N_LAYER_OUT];

  // Load data from file
  int rval = 1;
  rval += read_file_1D<input_t, N_LAYER_IN>("data/validation_data_784x1.dat", data);
  rval += read_file_2D<coeff_t, N_LAYER_IN,  N_LAYER1_OUT>("data/mnist_layer1_weights_small_784x256.dat", coeffs1);
  rval += read_file_1D<bias_t, N_LAYER1_OUT>("data/mnist_layer1_biases_small_256x1.dat", biases1);
  rval += read_file_2D<coeff_t, N_LAYER1_OUT, N_LAYER_OUT>("data/mnist_layer2_weights_small_256x10.dat", coeffs2);
  rval += read_file_1D<bias_t, N_LAYER_OUT>("data/mnist_layer2_biases_small_10x1.dat", biases2);
  rval += read_file_1D<float, N_LAYER_OUT>("data/validation_layer2_small_10x1.dat", answer);
  if (rval == 0) {
	  std::cout << "Failed to open files" << std::endl;
	  return -1;
  }

  // Run the basic neural net block
  result_t res[N_LAYER_OUT];
  nnet_2layer(data, coeffs1, biases1, coeffs2, biases2, res);

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


//int main () {
//  const int    SAMPLES=100;
//  FILE         *fp;
//
//  // short int taps[N] = {0,-10,-9,23,56,63,56,23,-9,-10,0,};
//
//  int i, ramp_up;
//  short int signal = 0;
//  ramp_up = 1;
//  axis in_data, out_data;
//
//  fp=fopen("out.dat","w");
//  // Stimulate with impulses
//  for (i=0;i<=SAMPLES;i++) {
//    if (ramp_up == 1)
//      signal = signal + 1;
//    else
//      signal = signal - 1;
//
//    // Populate input data
//    in_data.data = signal;
//    in_data.last = (i == SAMPLES);
//
//  // Execute the function with latest input
//    nnet_basic(in_data, out_data);
//
//   if ((ramp_up == 1) && (signal >= 75))
//    ramp_up = 0;
//   else if ((ramp_up == 0) && (signal <= -75))
//    ramp_up = 1;
//
//    // Save the results.
//     printf("In: %i    Out: %i   Last: %i \n", in_data.data, out_data.data, (int)out_data.last);
//    // fprintf(fp,"%i %i %i\n",  out_data.data.real(), out_data.data.imag(), (int)out_data.last);
//  }
//  fclose(fp);
//
// //  printf ("Comparing against output data \n");
// //  if (system("diff -w out.dat out.gold.dat")) {
//
//  // fprintf(stdout, "*******************************************\n");
//  // fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
//  // fprintf(stdout, "*******************************************\n");
// //     return 1;
// //  } else {
//  // fprintf(stdout, "*******************************************\n");
//  // fprintf(stdout, "PASS: The output matches the golden output!\n");
//  // fprintf(stdout, "*******************************************\n");
// //     return 0;
// //  }
//}
