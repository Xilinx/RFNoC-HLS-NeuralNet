#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "nnet_helpers.h"
#include "ex_modrec.h"

int main(int argc, char **argv)
{
  // Modulation recognition examples

  hls::stream<input_t> data_strm;
  for (int ii=0; ii < N_LAYER_IN; ii++) {
    data_strm << ii;
  }

  hls::stream<result_t> res_strm;
  unsigned short size_in, size_out;

  ex_modrec(data_strm, res_strm, size_in, size_out);

  std::cout << "Found network size: " << size_in << "x" << size_out << std::endl;

  // Print result vector
  int err_cnt = 0;
  float err, curr_data;
  for (int ii = 0; ii < N_LAYER_OUT; ii++) {
    curr_data = float(res_strm.read());
    // err = float(res[ii])-answer[ii];
    std::cout << "   Received: " << curr_data << std::endl;
    // std::cout << " Expected: " << answer[ii] << "   Received: " << res[ii] << "  ErrVal: " << err << std::endl;
    // if (abs(err) > 0.5) err_cnt++;
  }
  std::cout<< err_cnt << std::endl;
  return err_cnt;
}
