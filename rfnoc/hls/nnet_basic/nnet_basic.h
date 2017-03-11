#ifndef NNET_BASIC_H_
#define NNET_BASIC_H_

#include <complex>
#include "ap_int.h"

#define N  11

//typedef int	coef_t;
//typedef std::complex<short int> cplx_data_t;
//typedef int	acc_t;

typedef short int input_t;
typedef short int coeff_t;

struct axis {
    input_t data;
    ap_uint<1> last;
};

//  TODO: Add coefficients
void nnet_basic(axis &in,
		        axis &out);


#endif
