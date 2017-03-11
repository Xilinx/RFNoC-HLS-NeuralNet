#ifndef NNET_BASIC_H_
#define NNET_BASIC_H_

#include <complex>
#include "ap_int.h"

#define N  11

typedef short int input_t;
typedef short int coeff_t;
typedef short int bias_t;
typedef short int result_t;

struct axis {
    input_t data;
    ap_uint<1> last;
};


#define N_LAYER_IN  3
#define N_LAYER_OUT 3

// Prototype of top level function for C-synthesis
//  TODO: Add coefficients
void nnet_basic(
	  input_t data[N_LAYER_IN],
	  coeff_t weights[N_LAYER_IN][N_LAYER_OUT],
	  bias_t  biases[N_LAYER_OUT],
	  result_t res[N_LAYER_OUT]);
//void nnet_basic(axis &in,
//		        axis &out);

void nnet_layer(
	  input_t data[N_LAYER_IN],
      coeff_t weights[N_LAYER_IN][N_LAYER_OUT],
	  bias_t  biases[N_LAYER_OUT],
      result_t res[N_LAYER_OUT]);

#endif
