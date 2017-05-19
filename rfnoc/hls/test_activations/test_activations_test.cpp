#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "test_activations.h"

int verify_sigmoid(data_t data[N_LAYER], result_t res[N_LAYER])
{
  // Print result vector
  int err_cnt = 0;
  float ref, err, errtot;
  for (int ii = 0; ii < N_LAYER; ii++) {
    // Use Sigmoid for reference function...
    ref = 1.0 / (1 + exp(-float(data[ii])));
    err = float(res[ii]) - ref;
    errtot += err;
    std::cout << " Input: " << data[ii] << "   Expected: " << ref << "   Received: " << res[ii] << "  ErrVal: " << err << std::endl;
    if (abs(err) > 0.01) err_cnt++;
  }
  std::cout<< "Accum Errs: " << errtot << std::endl;
  std::cout<< err_cnt << std::endl;
  return err_cnt;
}

int verify_tanh(data_t data[N_LAYER], result_t res[N_LAYER])
{
  // Print result vector
  int err_cnt = 0;
  float ref, err, errtot;
  for (int ii = 0; ii < N_LAYER; ii++) {
    // Use Sigmoid for reference function...
    ref = tanh(float(data[ii]));
    err = float(res[ii]) - ref;
    errtot += err;
    std::cout << " Input: " << data[ii] << "   Expected: " << ref << "   Received: " << res[ii] << "  ErrVal: " << err << std::endl;
    if (abs(err) > 0.01) err_cnt++;
  }
  std::cout<< "Accum Errs: " << errtot << std::endl;
  std::cout<< err_cnt << std::endl;
  return err_cnt;
}

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

  return verify_sigmoid(data, res);
//   return verify_tanh(data, res);
}

