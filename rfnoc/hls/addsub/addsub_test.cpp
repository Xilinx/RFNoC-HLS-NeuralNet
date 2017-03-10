#include <stdio.h>
#include <math.h>
#include <complex>
#include "addsub.h"

int main () {
  const int    SAMPLES=100;
  FILE         *fp;

  axis_cplx in_a, in_b;
  axis_cplx add, sub;
  int signal = 0;
  int ramp_up = 1;

  fp=fopen("out.dat","w");
  for (int i=0; i<=SAMPLES; i++) {
  	if (ramp_up == 1) 
  		signal = signal + 1;
  	else 
  		signal = signal - 1;

  	in_a.data.real() = 0;
  	in_a.data.imag() = -signal;
  	in_a.last = (i==SAMPLES);
  	in_b.data.real() = signal;
  	in_b.data.imag() = signal;
  	in_b.last = (i==SAMPLES);

	// Execute the function with latest input
    addsub(in_a, in_b, add, sub);
    
    if ((ramp_up == 1) && (signal >= 100))
    	ramp_up = 0;
    else if ((ramp_up == 0) && (signal <= -100))
    	ramp_up = 1;
    	
	// Save the results.
    // printf("  A: %i+%ij  \t   B: %i+%ij\n", in_a.data.real(), in_a.data.imag(), in_b.data.real(), in_b.data.imag());
    // printf("Add: %i+%ij  \t Sub: %i+%ij\n", add.data.real(), add.data.imag(), sub.data.real(), sub.data.imag());
    fprintf(fp,"%i %i %i %i %i %i\n", add.data.real(), add.data.imag(), int(add.last),
    								  sub.data.real(), sub.data.imag(), int(sub.last));
  }
  fclose(fp);
  
  printf ("Comparing against output data \n");
  if (system("diff -w out.dat out.gold.dat")) {
	fprintf(stdout, "*******************************************\n");
	fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
	fprintf(stdout, "*******************************************\n");
     return 1;
  } else {
	fprintf(stdout, "*******************************************\n");
	fprintf(stdout, "PASS: The output matches the golden output!\n");
	fprintf(stdout, "*******************************************\n");
     return 0;
  }
}
