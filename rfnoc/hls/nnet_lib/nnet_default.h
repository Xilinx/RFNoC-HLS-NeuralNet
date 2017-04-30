#ifndef NNET_DEFAULT_H_
#define NNET_DEFAULT_H_

#include "ap_fixed.h"

// Default data types
typedef ap_fixed<16,4>  weight_t_def;
typedef ap_fixed<16,4>  bias_t_def;

// Default accumulator type
typedef ap_fixed<32,10> accum_t_def;

#endif
