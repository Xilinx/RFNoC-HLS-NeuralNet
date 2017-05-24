#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "test_conv.h"


int main(int argc, char **argv)
{

  // Populate test stimulus
  // data_t dataref_i[N_IN];
  // data_t dataref_q[N_IN];
  hls::stream<data_t> data_i, data_q;
  for (int ii = 0; ii < N_IN; ii++) {
    // dataref[ii] = ii;
    data_i << ii;
    data_q << ii;
  }

  // Run the basic neural net block
  hls::stream<result_t> res;
  test_conv(data_i, data_q, res);

  for (int ii = 0; ii < N_IN-N_FILT+1; ii++) {
    std::cout << "Received: " << res.read() << std::endl;
  }

//  return verify_sigmoid(dataref, res);
  // return verify_tanh(dataref, res);
}

