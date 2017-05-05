#ifndef EX_1LAYER_H_
#define EX_1LAYER_H_

#include <complex>
#include "ap_int.h"
#include "ap_fixed.h"

typedef ap_fixed<32,10> accum_t;
typedef ap_fixed<16,6> input_t;
typedef ap_fixed<12,3> coeff_t;
typedef ap_fixed<12,3> bias_t;
typedef ap_fixed<16,8> result_t;

#define N_LAYER_IN 784
#define N_LAYER_OUT 10

// Prototype of top level function for C-synthesis
void ex_1layer(
	  input_t data[N_LAYER_IN],
	  coeff_t weights[N_LAYER_IN][N_LAYER_OUT],
	  bias_t  biases[N_LAYER_OUT],
	  result_t res[N_LAYER_OUT]);

#endif
