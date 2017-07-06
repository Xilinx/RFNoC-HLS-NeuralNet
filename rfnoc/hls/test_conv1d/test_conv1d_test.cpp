//
//    rfnoc-hls-neuralnet: Vivado HLS code for neural-net building blocks
//
//    Copyright (C) 2017 EJ Kreinar
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

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
  data_t test1_data[TEST1_N_IN][TEST1_CHAN];
  hls::stream<data_t> data;
  for (int ii=0; ii<TEST1_N_IN; ii++){
    for (int jj=0; jj<TEST1_CHAN; jj++){
      test1_data[ii][jj] = (ii*TEST1_CHAN+jj)*0.01;
      data << test1_data[ii][jj];
    }
  }

  // Run the basic neural net block
  hls::stream<result_t> res;
  test_conv1d(data, res);

  float err, resval, refval;
  int errcount = 0;
  for (int ii = 0; ii < TEST1_N_OUT; ii++) {
    for (int jj = 0; jj < TEST1_CHAN; jj++) {
      resval = float(res.read());
      refval = jj*0.1+ 0.01*sum_of_n(ii*TEST1_CHAN+jj, ii*TEST1_CHAN+jj+TEST1_N_FILT*TEST1_CHAN, TEST1_CHAN);
      // refval = test1_ref[ii][jj];
      // refval = 0;
      err = resval - refval;
      std::cout << "Row/Chan: " << ii << "/" << jj  << ": " << resval << ", Ref: " << refval << ", Err: " << err << std::endl;
      if (abs(err)>0.0001)  errcount++;
    }
  }

  return  errcount;
}

int main_test2()
{
  data_t test2_data[TEST2_N_IN][TEST2_CHAN];
  hls::stream<data_t> data;
  for (int ii=0; ii<TEST2_N_IN; ii++){
    for (int jj=0; jj<TEST2_CHAN; jj++){
      test2_data[ii][jj] = (ii*TEST2_CHAN+jj)*0.0001;
      data << test2_data[ii][jj];
    }
  }

  // Run the basic neural net block
  hls::stream<result_t> res;
  test_conv1d(data, res);

  float err, resval, refval;
  int errcount = 0;
  for (int ii = 0; ii < TEST2_N_OUT; ii++) {
    for (int jj = 0; jj < TEST2_CHAN; jj++) {
      resval = float(res.read());
      refval = jj*0.01 + 0.0001*sum_of_n(ii*TEST2_CHAN+jj, ii*TEST2_CHAN+jj+TEST2_N_FILT*TEST2_CHAN, TEST2_CHAN);
      // refval = test1_ref[ii][jj];
      // refval = 0;
      err = resval - refval;
      std::cout << "Row/Chan: " << ii << "/" << jj  << ": " << resval << ", Ref: " << refval << ", Err: " << err << std::endl;
      if (abs(err)>0.0001)  errcount++;
    }
  }

  return errcount;
}


int main(int argc, char **argv)
{
  // NOTE: To run the different tests, make sure the function call
  //       below aligns with the function call in test_conv1d.cpp.
  //       I usually just comment/uncomment since this was mostly
  //       for testing & development...

  // RUN TEST1
  // return main_test1();

  // RUN TEST2
  return main_test2();
}

