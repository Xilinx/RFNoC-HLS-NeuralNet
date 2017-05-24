#include "nnet_conv.h"
#include "test_conv.h"


weight_t my_weights1[N_FILT][2][CHAN_OUT] =
          { { {1}, {1} },
            { {1}, {1} },
            { {1}, {1} },
            { {1}, {1} } };

bias_t   my_biases1[CHAN_OUT] = { 0.5 };

void test_conv(
      hls::stream<data_t>    &data_i,
      hls::stream<data_t>    &data_q,
      hls::stream<result_t>  &result)
{
    // Remove ap ctrl ports (ap_start, ap_ready, ap_idle, etc) since we only use the AXI-Stream ports
    #pragma HLS INTERFACE ap_ctrl_none port=return

    // ****************************************
    // NETWORK INSTATIATION
    // ****************************************

    // Activation function
    nnet::conv_iq<data_t, result_t, weight_t, bias_t, accum_t, N_IN, N_FILT, CHAN_OUT>(data_i, data_q, result, my_weights1, my_biases1);
}
