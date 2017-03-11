#include "fir_hls.h"

cplx_data_t coeff[N];

short int taps_real[N] = {0,-10,-9,23,56,63,56,23,-9,-10,0,};
short int taps_imag[N] = {0,-10,-9,23,56,63,56,23,-9,-10,0,};

// AXI-Stream port type is compatible with pointer, reference, & array input / ouputs only
// See UG902 Vivado High Level Synthesis guide (2014.4) pg 157 Figure 1-49
void fir_hls (axis_cplx &in, axis_cplx &out) {
//void fir_hls (axis_cplx &in, axis_cplx &out, cplx_data_t coeff[N]) {
    // Remove ap ctrl ports (ap_start, ap_ready, ap_idle, etc) since we only use the AXI-Stream ports
    #pragma HLS INTERFACE ap_ctrl_none port=return
	// Start this sucker fully pipelined- Seems to want 44 multipliers for N=11
	// ... which makes sense, because there's 4 x-ers per complex multiply
//	#pragma HLS PIPELINE
    // Set ports as AXI-Stream
    #pragma HLS INTERFACE axis port=in
    #pragma HLS INTERFACE axis port=out
	// Set format of Coeffs:
	#pragma HLS RESOURCE variable=coeff core=RAM_1P_BRAM
    // Need to pack our complex<short int> into a 32-bit word
    // Otherwise, compiler complains that our AXI-Stream interfaces have two data fields (i.e. data.real, data.imag)
    #pragma HLS DATA_PACK variable=in.data
    #pragma HLS DATA_PACK variable=out.data

	// Dummy loopback
	// out.data = in.data;

    // Filters!
	static cplx_data_t shift_reg[N];
	std::complex<short int> acc;
	cplx_data_t data;
	int ii;

	acc=0;
	Shift_Accum_Loop:
	for (ii=N-1;ii>=0;ii--) {
//#pragma HLS PIPELINE // <-- This fails to build/check out in RTL space
	#pragma HLS UNROLL factor=11
		if (ii==0) {
			shift_reg[0]=in.data;
			data = in.data;
		} else {
			shift_reg[ii]=shift_reg[ii-1];
			data = shift_reg[ii];
		}
		cplx_data_t coeff(taps_real[ii], taps_imag[ii]);
		acc += data*coeff;
	}
	out.data = (cplx_data_t) acc;


    // Pass through tlast
	out.last = in.last;
}
