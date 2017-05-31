#include "nnet_conv.h"
#include "test_conv1d.h"

//*******************************************************
// Demonstration Weights/Biases
// (hardcoded)
//*******************************************************

weight_t test1_weights[TEST1_N_FILT][TEST1_CHAN_IN] =
          { 1, 1, 1, 1,
            1, 1, 1, 1 };

bias_t   test1_biases[TEST1_CHAN_IN] = { 0.0, 0.1 };


//*******************************************************
// Top Level Function
//*******************************************************

void test_conv1d(
      hls::stream<data_t>    &data,
      hls::stream<result_t>  &result)
{
    // Remove ap ctrl ports (ap_start, ap_ready, ap_idle, etc) since we only use the AXI-Stream ports
    #pragma HLS INTERFACE ap_ctrl_none port=return

    // TEST 1
    nnet::conv_1d<data_t, result_t, weight_t, bias_t, accum_t, TEST1_N_IN, TEST1_CHAN_IN, TEST1_N_FILT>(data, result, test1_weights, test1_biases);

    // nnet::dummy_test<data_t, result_t, weight_t, bias_t, accum_t, TEST1_N_IN, TEST1_CHAN_IN, TEST1_N_FILT, TEST1_CHAN_OUT>(data, result, test1_weights, test1_biases);


    // // TEST 2
    // nnet::conv_iq<data_t, result_t, weight_t, bias_t, accum_t, TEST2_N_IN, TEST2_N_FILT, TEST2_CHAN_OUT>(data_i, data_q, result, test2_weights, test2_biases);


    // // TEST 3
    // nnet::conv_iq_manychan<data_t, result_t, weight_t, bias_t, accum_t, TEST3_N_IN, TEST3_N_FILT, TEST3_CHAN_OUT>(data_i, data_q, result, test3_weights, test3_biases);
}
