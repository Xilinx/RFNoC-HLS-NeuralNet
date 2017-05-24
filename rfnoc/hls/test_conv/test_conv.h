#ifndef TEST_ACTIVATIONS_H_
#define TEST_ACTIVATIONS_H_

#include <complex>
#include "ap_fixed.h"
#include "hls_stream.h"

// typedef ap_fixed<16,8>  data_t;
// typedef ap_fixed<16,2>  result_t;
// typedef ap_fixed<16,4>  weight_t;
// typedef ap_fixed<16,4>  bias_t;
// typedef ap_fixed<32,10> accum_t;

typedef float  data_t;
typedef float  result_t;
typedef float  weight_t;
typedef float  bias_t;
typedef float accum_t;




#define N_IN        10
#define N_FILT      4
#define CHAN_OUT    1

// Prototype of top level function for C-synthesis
void test_conv(
    hls::stream<data_t>   &data_i,
    hls::stream<data_t>   &data_q,
    hls::stream<result_t> &result);


#endif
