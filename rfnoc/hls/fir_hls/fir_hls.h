#ifndef FIR_H_
#define FIR_H_

#include <complex>
#include "ap_int.h"

#define N  11

//typedef int	coef_t;
typedef std::complex<short int> cplx_data_t;
//typedef int	acc_t;

struct axis_cplx {
    cplx_data_t data;
    ap_uint<1> last;
};

//void fir_hls(axis_cplx &in,
//		     axis_cplx &out,
//			 cplx_data_t coeff[N]
//		    );

void fir_hls(axis_cplx &in,
		     axis_cplx &out
		    );



#endif
