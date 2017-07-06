# HLS Code

The sub-projects in this repo fall into roughly 3 categories:

1. Neural Network Library: Contains the function definitions with compiler directives for efficient FPGA synthesis
    * nnet_lib
    
2. Code Examples: Contains working examples meant to demonstrate a relevant neural network instantiation
    * ex_1layer
    * ex_2layer
    * ex_modrec
    * ex_iqconv
    
3. Test Projects: These projects I used to develop and test a few of the nnet_lib headers. I decided to keep them here because I found them fairly useful to refer to at times.
    * test_activations
    * test_conv
    * test_conv1d

And finally, totally out of place, we have the "addsub" project, which I fixed up based on the HLS code in uhd-fpga and left as a convenient example. Should probably delete this. 
