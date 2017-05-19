#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "test_activations.h"

int main(int argc, char **argv)
{

  // Populate test stimulus
  data_t data[N_LAYER];
  for (int ii=0; ii<N_LAYER; ii++) {
    data[ii] = 10.0*(ii-float(N_LAYER)/2.0)/float(N_LAYER);
  }

  // Run the basic neural net block
  result_t res[N_LAYER];
  test_activations(data, res);

  // Print result vector
  int err_cnt = 0;
  float ref, err;
  for (int ii = 0; ii < N_LAYER; ii++) {
	ref = 1.0 / (1 + exp(-float(data[ii])));
	err = float(res[ii]) - ref;
    std::cout << " Input: " << data[ii] << "   Expected: " << ref << "   Received: " << res[ii] << "  ErrVal: " << err << std::endl;
    if (abs(err) > 0.01) err_cnt++;
  }
  std::cout<< err_cnt << std::endl;
  return 0;
}

