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

#include "test_activations.h"

int verify_sigmoid(data_t data[N_LAYER], hls::stream<result_t> &res)
{
  // Print result vector
  int err_cnt = 0;
  float ref, err, errtot;
  float dataval, resval;
  for (int ii = 0; ii < N_LAYER; ii++) {
    // Use Sigmoid for reference function...
	dataval = data[ii];
	resval  = float(res.read());
    ref = 1.0 / (1 + exp(-dataval));
    err = float(resval) - ref;
    errtot += err;
    std::cout << " Input: " << dataval << "   Expected: " << ref << "   Received: " << resval << "  ErrVal: " << err << std::endl;
    if (abs(err) > 0.01) err_cnt++;
  }
  std::cout<< "Accum Errs: " << errtot << std::endl;
  std::cout<< err_cnt << std::endl;
  return err_cnt;
}

int verify_tanh(data_t data[N_LAYER], hls::stream<result_t> &res)
{
  // Print result vector
  int err_cnt = 0;
  float ref, err, errtot;
  float dataval, resval;
  for (int ii = 0; ii < N_LAYER; ii++) {
    // Use tanh for reference function...
	dataval = data[ii];
	resval  = float(res.read());
    ref = tanh(float(dataval));
    err = float(resval) - ref;
    errtot += err;
    std::cout << " Input: " << dataval << "   Expected: " << ref << "   Received: " << resval << "  ErrVal: " << err << std::endl;
    if (abs(err) > 0.01) err_cnt++;
  }
  std::cout<< "Accum Errs: " << errtot << std::endl;
  std::cout<< err_cnt << std::endl;
  return err_cnt;
}

int main(int argc, char **argv)
{

  // Populate test stimulus
  hls::stream<data_t> data;
  data_t dataref[N_LAYER];
  for (int ii=0; ii<N_LAYER; ii++) {
	dataref[ii] = 10.0*(ii-float(N_LAYER)/2.0)/float(N_LAYER);
    data << dataref[ii];
  }

  // Run the basic neural net block
  hls::stream<result_t> res;
  test_activations(data, res);

//  return verify_sigmoid(dataref, res);
  return verify_tanh(dataref, res);
}

