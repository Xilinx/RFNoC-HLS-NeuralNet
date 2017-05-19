#ifndef TEST_ACTIVATIONS_H_
#define TEST_ACTIVATIONS_H_

#include <complex>
#include "ap_fixed.h"

typedef ap_fixed<16,8> data_t;
typedef ap_fixed<16,2> result_t;
//typedef float data_t;
//typedef float result_t;

#define N_LAYER 100

// Prototype of top level function for C-synthesis
void test_activations(
		data_t data[N_LAYER],
      result_t result[N_LAYER]);

#endif
