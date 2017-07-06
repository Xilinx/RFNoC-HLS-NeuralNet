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

#include "test_conv.h"


int main_test1()
{
  data_t test1_data[TEST1_N_IN][2] =
          { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9 };

  float test1_ref[TEST1_N_OUT][TEST1_CHAN_OUT] = 
          {12.5, 20.5, 28.5, 36.5, 44.5, 52.5, 60.5};

  hls::stream<data_t> data_i, data_q;
  for (int ii = 0; ii < TEST1_N_IN; ii++){
    data_i << test1_data[ii][0];
    data_q << test1_data[ii][1];
  }

  // Run the basic neural net block
  hls::stream<result_t> res;
  test_conv(data_i, data_q, res);

  float err, resval, refval;
  int errcount = 0;
  for (int ii = 0; ii < TEST1_N_OUT; ii++) {
    for (int jj = 0; jj < TEST1_CHAN_OUT; jj++) {
      resval = float(res.read());
      refval = test1_ref[ii][jj];
      err = resval - refval;
      std::cout << "Row: " << ii << ", Chan: " << jj << ": " << resval << ", Ref: " << refval << ", Err: " << err << std::endl;
      if (abs(err)>0.0001)  errcount++;
    }
  }

  return  errcount;
}

int main_test2()
{
  data_t test2_data[TEST2_N_IN][2] =
          { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9 };

  float test2_ref[TEST2_N_OUT][TEST2_CHAN_OUT] = 
          { 6*2+0.25,  6*4+0.50,  6*6+0.75, 
           10*2+0.25, 10*4+0.50, 10*6+0.75,
           14*2+0.25, 14*4+0.50, 14*6+0.75,
           18*2+0.25, 18*4+0.50, 18*6+0.75,
           22*2+0.25, 22*4+0.50, 22*6+0.75,
           26*2+0.25, 26*4+0.50, 26*6+0.75, 
           30*2+0.25, 30*4+0.50, 30*6+0.75};

  hls::stream<data_t> data_i, data_q;
  for (int ii = 0; ii < TEST2_N_IN; ii++){
    data_i << test2_data[ii][0];
    data_q << test2_data[ii][1];
  }

  // Run the basic neural net block
  hls::stream<result_t> res;
  test_conv(data_i, data_q, res);

  float err, resval, refval;
  int errcount = 0;
  for (int ii = 0; ii < TEST2_N_OUT; ii++) {
    for (int jj = 0; jj < TEST2_CHAN_OUT; jj++) {
      resval = float(res.read());
      refval = test2_ref[ii][jj];
      err = resval - refval;
      std::cout << "Row: " << ii << ", Chan: " << jj << ": " << resval << ", Ref: " << refval << ", Err: " << err << std::endl;
      if (abs(err)>0.0001)  errcount++;
    }
  }

  return  errcount;
}

float sum_of_n(float start, float  end){
	float sum =0;
	for (int ii=start; ii<end; ii++){
		sum+=ii;
	}
  return sum;
}

int main_test3()
{
  data_t test3_data[TEST3_N_IN][2];
  hls::stream<data_t> data_i, data_q;
  for (int ii=0; ii<TEST3_N_IN; ii++){
    test3_data[ii][0] = ii*0.0025;
    test3_data[ii][1] = -ii*0.0025;
    data_i << test3_data[ii][0];
    data_q << test3_data[ii][1];
  }

  // Run the basic neural net block
  hls::stream<result_t> res;
  test_conv(data_i, data_q, res);

  float err, resval, refval;
  int errcount = 0;
  for (int ii = 0; ii < TEST3_N_OUT; ii++) {
    for (int jj = 0; jj < TEST3_CHAN_OUT; jj++) {
      resval = float(res.read());
      refval = sum_of_n(ii,ii+TEST3_N_FILT)*0.005 + jj*0.01;
      err = resval - refval;
      std::cout << "Row: " << ii << ", Chan: " << jj << ": " << resval << ", Ref: " << refval << ", Err: " << err << std::endl;
      if (abs(err)>0.0001)  errcount++;
    }
  }

  return  errcount;
}

int main(int argc, char **argv)
{
  // NOTE: To run the different tests, make sure the function call
  //       below aligns with the function call in test_conv.cpp.
  //       I usually just comment/uncomment since this was mostly
  //       for testing & development...

  // RUN TEST1
  // return main_test1();

  // RUN TEST2
  // return main_test2();

  // RUN TEST3
  return main_test3();
}

