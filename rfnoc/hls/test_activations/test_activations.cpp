#include "test_activations.h"
#include "nnet_activation.h"


void test_activations(
      hls::stream<data_t>    &data,
      hls::stream<result_t>  &result)
{
    // Remove ap ctrl ports (ap_start, ap_ready, ap_idle, etc) since we only use the AXI-Stream ports
    #pragma HLS INTERFACE ap_ctrl_none port=return

    // ****************************************
    // NETWORK INSTATIATION
    // ****************************************

    // Activation function
//    nnet::sigmoid<data_t, result_t, N_LAYER, 256>(data, result);
    nnet::tanh<data_t, result_t, N_LAYER>(data, result);
}
