
#include "nnet_conv.h"
#include "nnet_activation.h"
#include "nnet_layer.h"
#include "nnet_helpers.h"
#include "nnet_pooling.h"

#include "ex_iqconv.h"

void get_consts_1(weight_t test3_weights[N_IQCONV_FILT][2*N_IQCONV_CHAN], bias_t test3_biases[N_IQCONV_CHAN]){
  for (int ii=0; ii<N_IQCONV_FILT; ii++){
    for (int jj=0; jj<N_IQCONV_CHAN; jj++){
      test3_weights[ii][2*jj]=1.0;
      test3_weights[ii][2*jj+1]=-1.0;
    }
  }

  for (int ii=0; ii<N_IQCONV_CHAN; ii++) test3_biases[ii]=ii*0.01;
}


// AXI-Stream port type is compatible with pointer, reference, & array input / ouputs only
// See UG902 Vivado High Level Synthesis guide (2014.4) pg 157 Figure 1-49
void ex_iqconv(
      hls::stream<input_t> &data_i,
      hls::stream<input_t> &data_q,
      hls::stream<result_t> &res,
      unsigned short &const_size_in,
      unsigned short &const_size_out)
{
    // Remove ap ctrl ports (ap_start, ap_ready, ap_idle, etc) since we only use the AXI-Stream ports
    #pragma HLS INTERFACE ap_ctrl_none port=return

    // Connect size indicators
    #pragma HLS INTERFACE axis port=data
    #pragma HLS INTERFACE axis port=res
    #pragma HLS INTERFACE ap_none port=const_size_in
    #pragma HLS INTERFACE ap_none port=const_size_out
    const_size_in   = N_LAYER_IN;
    const_size_out  = N_LAYER_OUT;

    #pragma HLS DATAFLOW

    // ****************************************
    // NETWORK INSTATIATION
    // ****************************************

    // IQ Convolution
    weight_t w1 [N_IN][2*N_IQCONV_CHAN];
    bias_t   b1 [N_IQCONV_CHAN];
    get_consts_1(w1, b1);

    hls::stream<result_t> logits1, hidden1;
    nnet::conv_iq<input_t, result_t, weight_t, bias_t, accum1_t, N_IN, N_IQCONV_FILT, N_IQCONV_CHAN>
                 (data_i, data_q, logits1, w1, b1);
    nnet::relu<result_t, result_t, N_IQCONV_FLAT>(logits1, hidden1);

    // Maxpool operation
    hls::stream<result_t> maxpool1;
    nnet::maxpool_2x<result_t, N_IQCONV_OUT, N_IQCONV_CHAN>(hidden1, res);
}
