#include <stdio.h>
#include <math.h>
#include "nnet_basic.h"

int main () {
  const int    SAMPLES=100;
  FILE         *fp;

  // short int taps[N] = {0,-10,-9,23,56,63,56,23,-9,-10,0,};

  int i, ramp_up;
  short int signal = 0;
  ramp_up = 1;
  axis in_data, out_data;
  
  fp=fopen("out.dat","w");
  // Stimulate with impulses
  for (i=0;i<=SAMPLES;i++) {
  	if (ramp_up == 1) 
  		signal = signal + 1;
  	else 
  		signal = signal - 1;

  	// Populate input data
  	in_data.data = signal;
  	in_data.last = (i == SAMPLES);

	// Execute the function with latest input
  	nnet_basic(in_data, out_data);
    
   if ((ramp_up == 1) && (signal >= 75))
   	ramp_up = 0;
   else if ((ramp_up == 0) && (signal <= -75))
   	ramp_up = 1;
    	
		// Save the results.
     printf("In: %i    Out: %i   Last: %i \n", in_data.data, out_data.data, (int)out_data.last);
    // fprintf(fp,"%i %i %i\n",  out_data.data.real(), out_data.data.imag(), (int)out_data.last);
  }
  fclose(fp);
  
 //  printf ("Comparing against output data \n");
 //  if (system("diff -w out.dat out.gold.dat")) {

	// fprintf(stdout, "*******************************************\n");
	// fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
	// fprintf(stdout, "*******************************************\n");
 //     return 1;
 //  } else {
	// fprintf(stdout, "*******************************************\n");
	// fprintf(stdout, "PASS: The output matches the golden output!\n");
	// fprintf(stdout, "*******************************************\n");
 //     return 0;
 //  }
}
