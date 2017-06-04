# rfnoc-machinelearn

Open-source components for implementing neural networks in FPGA fabric. 

- Uses the RF Network-on-Chip ([RFNOC](https://www.ettus.com/sdr-software/detail/rf-network-on-chip)) framework for the software/FPGA IO interfaces. 
- Uses [Vivado HLS](https://www.xilinx.com/products/design-tools/vivado/integration/esl-design.html) to generate custom HDL.

Designed for RF processing, though the HLS portion may be more generally applicable to creating other neural nets, too.

The neural network library (rfnoc-neuralnet) provides an RFNoC OOT module for efficiently deploying a trained neural network to an FPGA. Typically, neural networks are designed, trained, and executed on a conventional processor, often with GPU acceleration. But for embedded devices which may need to process data at multiple-MHz sample rates, the computational requirements can be overwhelming for an embedded processor where no GPU is available, creating a tempting opportunity for FPGA acceleration.

The neural network library here provides pre-optimized neural network building blocks using the Vivado HLS tool to synthesize the desired neural network. RFNoC provides a convenient input/output interface between hardware and software that is compatible with gnuradio. Ideally, the neural network designer will be able to deploy neural networks and evaluate resource vs throughput tradeoffs without needing to develop and maintain the repetitive “glue code” in FPGA and software. Presented examples demonstrate various use-cases in a simulation environment and on the E310, including image classification and modulation recognition, using both fully-connected and convolutional layers.

The HLS neural network library currently supports the following constructs:
 * Fully connected layer
   * Multiple HLS optimization based on the size of the layer
 * 1-Dimensiona convolutional layer with multiple input channels
 * IQ convolution layer with multiple output channels
   * Notionally optimized for RF data
   * 1-D layer with 2 simultaneous input channels
 * Various activation functions
   * relu
   * relu6
   * sigmoid
   * tanh
 * Maxpool operation

## Goals

A neural network cannot be provided as a “one-size-fits-all” solution; the designer’s neural network architecture is a major driver of performance, and the variety of possible architectures is far too large to provide a generalized FPGA solution. Any two neural networks will have dramatically different architectures, including different flavors of convolutional or fully-connected layers, different activation functions, etc. This paradigm essentially reduces the impact of any pre-generated FPGA-based neural network designed fora specific network architecture.

Using a software-only approach, changing the neural net architecture is trivial. However, in hardware, FPGA fabric cannot be arbitrarily reconfigured on the fly; the desired architecture must be synthesized and implemented ahead of time for the optimal tradeoff of resource usage, compute time, and power. Therefore, the true strength of an FPGA-based neural network architecture would be the ability for the designer to regenerate a resource-efficient FPGA implementation in a short amount of time without needing to reinvent the wheel. Based on these assertions, the goals for the RFNoC Neural Network project are as follows:

* Develop an HLS library of common neural network functions
  * The library will include options such as different layers, activation functions, convolutional layers, etc.
  * The neural network functions can be combined in a conceptually similar process to using tensorflow. Using the C syntax in HLS, designers will instantiate the desired network architecture, test their implementation using HLS tools, and synthesize to HDL.

* Provide the FPGA architecture to wrap generated HDL code into an RFNoC compute engine (CE).
  * Use the AXI Stream interface to send samples to the CE

* Provide a software interface. The software will interact with the RFNoC CE to:
  * Interface with the AXI Stream inputs and outputs
  * Provide a gnuradio-companion (GRC) interface
  * Program the neural net weights/biases at run time (not currently supported, but a nice-to-have future feature)

* Develop a demonstration application using the HLS + RFNoC + GRC workflow.

## Pre-Reqs

[updated 6/3/2017. TBD what stays here depending on merge actions and such...] 

Running simulations and builds on the rfnoc-neuralnet OOT module requires some edits to the uhd-fpga repo.

First, a number of edits to uhd-fpga's Makefile system are required to handle HLS simulations in the repo. These updates will let you run "make xsim_hls" correctly in the OOT module's testbenches folder. I have a fork of uhd-fpga here that supports these updates: https://github.com/ejk43/fpga/tree/ejk/hls_testbench

Second, additional edits to uhd-fpga's build system are required to provide OOT modules with the extra flexibility to generate HLS and then plug into a new FPGA build. These updates will let you point to Makefiles in an OOT module, which can then generate IP or HLS, when running "make E310_RFNOC", "make X310_RFNOC_HG", etc. These edits are forked here: https://github.com/ejk43/fpga/tree/new_oot_includes

For the time being, I would recommend merging or cherry picking these updates into uhd-fpga. 

## Examples

In rfnoc-neuralnet/examples, there are a few pre-generated example RFNoC blocks that have been developed and tested against the E310 hardware platform. 

#### noc_block_ex1layer

The neural network in this RFNoC block is modeled after the [first Tensorflow example](https://github.com/tensorflow/tensorflow/tree/master/tensorflow/examples/udacity) (specifically, 2_fullyconnected.ipynb) in the online [Udacity course on Deep Learning](https://www.udacity.com/course/deep-learning--ud730). This is a relatively simple one-layer fully-connected network, which I put together from the example notebook, then saved the pretrained weights and biases. The network is designed to accept 28 x 28 grayscale images from the "not_MNIST" dataset and to classify into 10 categories representing letters A-J.

**Running RFNoC Testbench:** Inside rfnoc-neuralnet/rfnoc/testbenches/noc_block_ex1layer, run "make xsim_hls". The testbench should run and pass.

**Running HLS Testbench:** To run the HLS example, follow the readme in rfnoc-neuralnet/rfnoc/hls/ex_1layer. When the project is open, run the C Simulation and C Synthesis to observe synthesized results.

**Running GRC Flowgraph:** Open and compile rfnoc-neuralnet/examples/nnet_example_1layer.grc. To run on the E310, transfer the necessary cross-compiled prefix, the generated python flowgraph, and the dataset mnist_data_input.s16 to the E310. The mnist_data_input.s16 file contains a single image which is equivalent to the test data used in the HLS and RFNoC testbenches, and can be used as a regression-check on the E310. On the E310, run the generated python file. This should create a new file output "mnist_rfnoc_output.s16"-- you should be able to verify the contents of mnist_rfnoc_output.s16 exactly match the RFNoC testbench output. 

Optionally, in the nnet_example_1layer.grc flowgraph, enable the file source for ex1_binary_validation_data.s16. This contains all the validation data from the Udacity tensorflow course. I've saved this data on [my google drive](https://drive.google.com/open?id=0B4jB60EiIiLPLU95X3Jja280M0E) in order to avoid putting it on the git repo. Move the validation data to the E310 and run the python flowgraph pointing to the larger validation set. 
