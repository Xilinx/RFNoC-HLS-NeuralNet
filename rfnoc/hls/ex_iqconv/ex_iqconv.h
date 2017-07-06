#ifndef EX_IQCONV_H_
#define EX_IQCONV_H_

#include <complex>
#include "ap_int.h"
#include "ap_fixed.h"
#include "hls_stream.h"

typedef ap_fixed<16,4> input_t;
typedef ap_fixed<16,8> result_t;
typedef ap_fixed<16,8> weight_t;
typedef ap_fixed<16,8> bias_t;

typedef ap_fixed<32,12> accum1_t;
typedef ap_fixed<16,8> layerIQ_t;

typedef ap_fixed<32,12> accum2_t;
typedef ap_fixed<16,8> layer1D_t;

typedef ap_fixed<16,8> layer1_t;
typedef ap_fixed<16,8> layer2_t;
typedef ap_fixed<16,8> layer3_t;
typedef ap_fixed<16,8> layer4_t;
typedef ap_fixed<16,8> layer5_t;


// typedef float input_t;
// typedef float result_t;
// typedef float weight_t;
// typedef float bias_t;

// typedef float accum1_t;
// typedef float layerIQ_t;

// typedef float accum2_t;
// typedef float layer1D_t;

// typedef float accumX_t;
// typedef float layer1_t;
// typedef float layer2_t;
// typedef float layer3_t;
// typedef float layer4_t;
// typedef float layer5_t;

#define N_IN            128
#define N_IQCONV_FILT   8
#define N_IQCONV_CHAN   128
#define N_IQCONV_OUT    121
#define N_IQCONV_FLAT   15488

#define MAXPOOL_SIZE_1   2
#define MAXPOOL_STRIDE_1 2
#define N_MAXPOOL_FLAT_1 3840

#define N_1DCONV_IN     60
#define N_1DCONV_FILT   16
#define N_1DCONV_CHAN   64
#define N_1DCONV_OUT    45
#define N_1DCONV_FLAT   2880

#define MAXPOOL_SIZE_2   2
#define MAXPOOL_STRIDE_2 2
#define N_MAXPOOL_FLAT_2 704

#define N_LAYER_IN      704
#define N_LAYER_1       128
#define N_LAYER_2       64
#define N_LAYER_3       32
#define N_LAYER_4       10
#define N_LAYER_OUT     N_LAYER_4

// Prototype of top level function for C-synthesis
void ex_iqconv(
      hls::stream<input_t> &data_i,
      hls::stream<input_t> &data_q,
      hls::stream<result_t> &res,
      unsigned short &const_size_in,
      unsigned short &const_size_out);

#endif
