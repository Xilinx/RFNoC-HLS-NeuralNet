#include "ex_activations.h"
#include "nnet_activation.h"


void ex_activations(
      data_t    data[N_LAYER],
      result_t  result[N_LAYER])
{
    // Remove ap ctrl ports (ap_start, ap_ready, ap_idle, etc) since we only use the AXI-Stream ports
    #pragma HLS INTERFACE ap_ctrl_none port=return

    // ****************************************
    // NETWORK INSTATIATION
    // ****************************************

    // Activation function
    nnet::sigmoid<data_t, result_t, N_LAYER>(data, result);
}
