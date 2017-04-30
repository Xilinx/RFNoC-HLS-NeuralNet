#ifndef NNET_HELPERS_H
#define NNET_HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

template <class dataType>
int read_file_1D(const char * filename, dataType *data, unsigned int nsamps)
{
  FILE *fp;
//  data_t datain[nsamps];

  fp = fopen(filename, "r");
  //std::cout<< "Reading 1D file: " << fp << std::endl;
  if (fp == 0) {
    //std::cout << "Failed to open file" << std::endl;
    return 1;
  }
  // Read data from file
  float newval;
  for (int ii = 0; ii < nsamps; ii++){
    if (fscanf(fp, "%f\n", &newval) != 0){
      data[ii] = newval;
    } else {
      //std::cout << "File failed to extract data" << std::endl;
      return 2;
    }
  }
  fclose(fp);
  return 0;
}

template <class dataType>
int read_file_2D(const char * filename, dataType data[], unsigned int nrows, unsigned int ncols)
{
  FILE *fp;
//  data_t datain[ncols][nrows];

  fp = fopen(filename, "r");
  //std::cout<< "Reading 2D file: " << fp << std::endl;
  if (fp == 0) {
    //std::cout << "Failed to open file" << std::endl;
    return 1;
  }
  // Read data from file
  float newval;
  for (int ii = 0; ii < nrows; ii++) {
    for (int jj = 0; jj < ncols; jj++){
      if (fscanf(fp, "%f\n", &newval) != 0){
    	//std::cout<<"Reading " << ii << " " << jj << " " << newval << std::endl;
        data[ii*ncols+jj] = newval;
      } else {
        //std::cout << "File failed to extract data" << std::endl;
        return 2;
      }
    }
  }
  fclose(fp);
  return 0;
}

#endif
