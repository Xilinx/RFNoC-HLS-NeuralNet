#ifndef NNET_HELPERS_H
#define NNET_HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

template <class dataType>
int read_file_1D(const char * filename, dataType *data, unsigned int nsamps)
{
  FILE *fp;
  fp = fopen(filename, "r");
  if (fp == 0) {
    return -1;
  }
  // Read data from file
  float newval;
  for (int ii = 0; ii < nsamps; ii++){
    if (fscanf(fp, "%f\n", &newval) != 0){
      data[ii] = newval;
    } else {
      return -2;
    }
  }
  fclose(fp);
  return 0;
}

template <class dataType>
int read_file_2D(const char * filename, dataType data[], unsigned int nrows, unsigned int ncols)
{
  FILE *fp;
  fp = fopen(filename, "r");
  if (fp == 0) {
    return -1;
  }
  // Read data from file
  float newval;
  for (int ii = 0; ii < nrows; ii++) {
    for (int jj = 0; jj < ncols; jj++){
      if (fscanf(fp, "%f\n", &newval) != 0){
        data[ii*ncols+jj] = newval;
      } else {
        return -2;
      }
    }
  }
  fclose(fp);
  return 0;
}

#endif
