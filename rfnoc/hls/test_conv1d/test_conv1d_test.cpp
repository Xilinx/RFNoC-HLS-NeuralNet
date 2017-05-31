#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "test_conv1d.h"

float sum_of_n(float start, float  end, float increment){
  float sum =0;
  // std::cout<<"Running Sum"<<std::endl;
  for (int ii=start; ii<end; ii=ii+increment){
    sum+=ii;
    // std::cout<<sum<<std::endl;
  }
  return sum;
}

int main_test1()
{
  data_t test1_data[TEST1_N_IN][TEST1_CHAN_IN];
  hls::stream<data_t> data;
  for (int ii=0; ii<TEST1_N_IN; ii++){
    for (int jj=0; jj<TEST1_CHAN_IN; jj++){
      test1_data[ii][jj] = (ii*TEST1_CHAN_IN+jj)*0.01;
      data << test1_data[ii][jj];
    }
  }

  // Run the basic neural net block
  hls::stream<result_t> res;
  test_conv1d(data, res);

  float err, resval, refval;
  int errcount = 0;
  for (int ii = 0; ii < TEST1_N_OUT; ii++) {
    for (int jj = 0; jj < TEST1_CHAN_IN; jj++) {
      for (int kk = 0; kk < TEST1_CHAN_OUT; kk++) {
        resval = float(res.read());
        refval = jj*0.1+ 0.01*sum_of_n(ii*TEST1_CHAN_IN+jj, ii*TEST1_CHAN_IN+jj+TEST1_N_FILT*TEST1_CHAN_IN, TEST1_CHAN_IN);
        // refval = test1_ref[ii][jj];
        // refval = 0;
        err = resval - refval;
        std::cout << "Row/ChIn/ChOut: " << ii << "/" << jj << "/" << kk << ": " << resval << ", Ref: " << refval << ", Err: " << err << std::endl;
        if (abs(err)>0.0001)  errcount++;
      }
    }
  }

  return  errcount;
}


int main(int argc, char **argv)
{
  // RUN TEST1
  return main_test1();

  // // RUN TEST2
  // return main_test2();

  // // RUN TEST3
  // return main_test3();
}

