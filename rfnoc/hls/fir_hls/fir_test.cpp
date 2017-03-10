#include <stdio.h>
#include <math.h>
#include "fir_hls.h"

int main () {
  const int    SAMPLES=100;
  FILE         *fp;

  short int taps_real[N] = {0,-10,-9,23,56,63,56,23,-9,-10,0,};
  short int taps_imag[N] = {0,-10,-9,23,56,63,56,23,-9,-10,0,};
  cplx_data_t taps[N];
  for (int ii=0; ii<N; ii++) {
	  taps[ii].real() = taps_real[ii];
	  taps[ii].imag() = taps_imag[ii];
  }

  int i, ramp_up;
  short int signal = 0;
  ramp_up = 1;
  axis_cplx in_data, out_data;
  
//  fp=fopen("out.dat","w");
  // Stimulate with impulses
  for (i=0;i<=SAMPLES;i++) {
	if (i%20 == 0)
		signal = 1;
	else
		signal = 0;

  	// Populate input data
  	in_data.data.real() = signal;
  	in_data.data.imag() = 0;
  	in_data.last = (i == SAMPLES);

	// Execute the function with latest input
  	fir_hls(in_data, out_data, taps);
    
//    if ((ramp_up == 1) && (signal >= 75))
//    	ramp_up = 0;
//    else if ((ramp_up == 0) && (signal <= -75))
//    	ramp_up = 1;
    	
		// Save the results.
     printf("In: %i+%ij  \t Out: %i+%ij\n", in_data.data.real(), in_data.data.imag(), out_data.data.real(), out_data.data.imag());
//    fprintf(fp,"%i %d %d\n",i,signal,output);
  }
//  fclose(fp);
  
//  printf ("Comparing against output data \n");
//  if (system("diff -w out.dat out.gold.dat")) {
//
//	fprintf(stdout, "*******************************************\n");
//	fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
//	fprintf(stdout, "*******************************************\n");
//     return 1;
//  } else {
//	fprintf(stdout, "*******************************************\n");
//	fprintf(stdout, "PASS: The output matches the golden output!\n");
//	fprintf(stdout, "*******************************************\n");
//     return 0;
//  }
}
