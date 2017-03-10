#ifndef ADDSUB_H_
#define ADDSUB_H_

#include "ap_int.h"

#define N	11

//typedef int	coef_t;
//typedef short int	data_t;
//typedef int	acc_t;

struct axis_cplx {
    std::complex<short int> data;
    ap_uint<1> last;
};

void addsub(axis_cplx &a, axis_cplx &b,
		    axis_cplx &add, axis_cplx &sub);



#endif
