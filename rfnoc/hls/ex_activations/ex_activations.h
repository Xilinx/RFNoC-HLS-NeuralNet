#ifndef EX_1LAYER_H_
#define EX_1LAYER_H_

#include <complex>
#include "ap_fixed.h"

typedef ap_fixed<16,8> data_t;
typedef ap_fixed<16,2> result_t;
//typedef float data_t;
//typedef float result_t;

#define N_LAYER 100

// Prototype of top level function for C-synthesis
void ex_activations(
		data_t data[N_LAYER],
      result_t result[N_LAYER]);

#endif
