#ifndef NNET_BASIC_H_
#define NNET_BASIC_H_

#include <complex>
#include "ap_int.h"
#include "ap_fixed.h"

// TODO: convert data types to fixed point (bleh)

//typedef short int data_t;
//typedef float data_t;
typedef ap_fixed<16,6> data_t;

typedef ap_fixed<32,12> accum_t;


typedef data_t input_t;
typedef data_t coeff_t;
typedef data_t bias_t;
typedef data_t result_t;

// TODO: Figure out if we're using axi-stream interfaces?
struct axis {
    input_t data;
    ap_uint<1> last;
};


#define N_LAYER_IN 784
#define N_LAYER_OUT 10

//#define N_LAYER_IN  3
//#define N_LAYER_OUT 3

// Prototype of top level function for C-synthesis
void nnet_basic(
	  input_t data[N_LAYER_IN],
	  coeff_t weights[N_LAYER_IN][N_LAYER_OUT],
	  bias_t  biases[N_LAYER_OUT],
	  result_t res[N_LAYER_OUT]);

void nnet_layer(
	  input_t data[N_LAYER_IN],
      coeff_t weights[N_LAYER_IN][N_LAYER_OUT],
	  bias_t  biases[N_LAYER_OUT],
      result_t res[N_LAYER_OUT]);

#endif
