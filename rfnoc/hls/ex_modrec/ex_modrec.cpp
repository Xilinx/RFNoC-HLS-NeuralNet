
#include "nnet_layer.h"
#include "nnet_activation.h"

#include "ex_modrec.h"
// #include "data/ex_modrec_weights.h"
#include "data/ex_modrec_consts.h"

template<class data_T, int N_IN>
void  hls_stream_debug(hls::stream<data_T> &data, hls::stream<data_T> &res)
{
    data_T datareg;
    for (int ii=0; ii<N_IN; ii++) {
        datareg = data.read();
        std::cout << "[" << ii << "]: " << datareg << std::endl;
        res << datareg;
    }
}

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
    const_size_out  = N_LAYER_OUT;

    #pragma HLS DATAFLOW

    // ****************************************
    // NETWORK INSTATIATION
    // ****************************************

    // LAYER 1
    hls::stream<layer1_t> logits1, hidden1;
    nnet::compute_layer<input_t, layer1_t, weight_t, bias_t, accum_t, N_LAYER_IN, N_LAYER_1>(data, logits1, w1, b1);
    nnet::relu6<layer1_t, layer1_t, N_LAYER_1>(logits1, hidden1);

    // LAYER 2
    hls::stream<layer2_t> logits2, hidden2;
    nnet::compute_layer<layer1_t, layer2_t, weight_t, bias_t, accum_t, N_LAYER_1, N_LAYER_2>(hidden1, logits2, w2, b2);
    nnet::relu6<layer2_t, layer2_t, N_LAYER_2>(logits2, hidden2);

    // LAYER 3
    hls::stream<layer3_t> logits3, hidden3;
    nnet::compute_layer<layer2_t, layer3_t, weight_t, bias_t, accum_t, N_LAYER_2, N_LAYER_3>(hidden2, logits3, w3, b3);
    nnet::relu6<layer3_t, layer3_t, N_LAYER_3>(logits3, hidden3);

    // LAYER 4
    hls::stream<layer4_t> logits4, hidden4;
    nnet::compute_layer<layer3_t, layer4_t, weight_t, bias_t, accum_t, N_LAYER_3, N_LAYER_4>(hidden3, logits4, w4, b4);
    nnet::relu6<layer4_t, layer4_t, N_LAYER_4>(logits4, hidden4);

    // LAYER 5
    nnet::compute_layer<layer4_t, layer5_t, weight_t, bias_t, accum5_t, N_LAYER_4, N_LAYER_5>(hidden4, res, w5, b5);
}
