
#include "nnet_layer.h"
#include "nnet_activation.h"

#include "ex_modrec.h"
#include "data/ex_modrec_weights.h"

// AXI-Stream port type is compatible with pointer, reference, & array input / ouputs only
// See UG902 Vivado High Level Synthesis guide (2014.4) pg 157 Figure 1-49
void ex_modrec(
      hls::stream<input_t> &data,
      hls::stream<result_t> &res,
      unsigned short &const_size_in,
      unsigned short &const_size_out)
{
    // Remove ap ctrl ports (ap_start, ap_ready, ap_idle, etc) since we only use the AXI-Stream ports
    #pragma HLS INTERFACE ap_ctrl_none port=return

    // Connect size indicators
    #pragma HLS INTERFACE ap_none port=const_size_in
    #pragma HLS INTERFACE ap_none port=const_size_out
    const_size_in   = N_LAYER_IN;
    const_size_out  = N_LAYER_3;

    // ****************************************
    // NETWORK INSTATIATION
    // ****************************************

    // LAYER 1
    hls::stream<layer1_t> logits1, hidden1;
    nnet::compute_layer<input_t, layer1_t, weight_t, bias_t, accum_t, N_LAYER_IN, N_LAYER_1>(data, logits1, lay1_weights, lay1_biases);
    nnet::relu6<layer1_t, layer1_t, N_LAYER_1>(logits1, hidden1);

    // LAYER 2
    hls::stream<layer2_t> logits2, hidden2;
    nnet::compute_layer<layer1_t, layer2_t, weight_t, bias_t, accum_t, N_LAYER_1, N_LAYER_2>(hidden1, logits2, lay2_weights, lay2_biases);
    nnet::relu6<layer2_t, layer2_t, N_LAYER_2>(logits2, hidden2);

    // LAYER 3
    hls::stream<layer3_t> logits3, hidden3;
    nnet::compute_layer<layer2_t, layer3_t, weight_t, bias_t, accum_t, N_LAYER_2, N_LAYER_3>(hidden2, logits3, lay3_weights, lay3_biases);
    nnet::relu6<layer3_t, result_t, N_LAYER_3>(logits3, res);
}
