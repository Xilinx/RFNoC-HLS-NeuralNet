# rfnoc-machinelearn

Open-source components for implementing neural networks in FPGA fabric. 

- Uses the RF Network-on-Chip ([RFNOC](https://www.ettus.com/sdr-software/detail/rf-network-on-chip)) framework for the software/FPGA IO interfaces. 
- Uses [Vivado HLS](https://www.xilinx.com/products/design-tools/vivado/integration/esl-design.html) to generate custom HDL.

Designed for RF processing, though the HLS portion may be more generally applicable to creating other neural nets, too.

The HLS neural network library currently supports the following constructs:
 * Fully connected layer
   * Multiple HLS optimization based on the size of the layer
 * 1-Dimensional convolutional layer with multiple input channels
 * IQ convolution layer with multiple output channels
   * Notionally optimized for RF data
   * 1-D layer with 2 simultaneous input channels
 * Various activation functions
   * relu
   * relu6
   * sigmoid
   * tanh
 * Maxpool operation (size 2, stride 2)

## Summary

The neural network library (rfnoc-neuralnet) provides an RFNoC OOT module for efficiently deploying a trained neural network to an FPGA. Typically, neural networks are designed, trained, and executed on a conventional processor, often with GPU acceleration. But for embedded devices which may need to process data at multiple-MHz sample rates, the computational requirements can be overwhelming for an embedded processor where no GPU is available, creating a tempting opportunity for FPGA acceleration.

The neural network library here provides pre-optimized neural network building blocks using the Vivado HLS tool to synthesize the desired neural network. RFNoC provides a convenient input/output interface between hardware and software that is compatible with gnuradio. Ideally, the neural network designer will be able to deploy neural networks and evaluate resource vs throughput tradeoffs without needing to develop and maintain the repetitive “glue code” in FPGA and software. Presented examples demonstrate various use-cases in a simulation environment and on the E310, including image classification and modulation recognition, using both fully-connected and convolutional layers.

## Goals

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

# Instructions

[updated 6/3/2017. TBD what stays here depending on merge actions and such...] 

Running simulations and builds on the rfnoc-neuralnet OOT module requires some edits to the uhd-fpga repo.

First, a number of edits to uhd-fpga's Makefile system are required to handle HLS simulations in the repo. These updates will let you run "make xsim_hls" correctly in the OOT module's testbenches folder. I have a fork of uhd-fpga here that supports these updates: https://github.com/ejk43/fpga/tree/ejk/hls_testbench

Second, additional edits to uhd-fpga's build system are required to provide OOT modules with the extra flexibility to generate HLS and then plug into a new FPGA build. These updates will let you point to Makefiles in an OOT module, which can then generate IP or HLS, when running "make E310_RFNOC", "make X310_RFNOC_HG", etc. These edits are forked here: https://github.com/ejk43/fpga/tree/new_oot_includes

For the time being, I would recommend merging or cherry picking these updates into uhd-fpga. 

## Workflow

The HLS-RFNoC workflow is a bit involved, yet worthwhile. 

1) **HLS Floating-point Simulation**: In the initial stage, the developer first implements and simulates the desired neural network using floating point datatypes in C++ using the provided code from rfnoc-hls-neuralnet. The general goal in this step is to identically replicate the performance of the selected neural network algorithm in the HLS framework. See the HLS readme for more detailed information. 

    Note that while the neural network code is relatively straightforward, there two major differences between the HLS-optimized code and a typical C or C++ implementation. First, the input/output interface to each function uses Vivado's hls_stream object, which provides a FIFO buffer that helps developers program C++ code that is naturally conducive to FPGA synthesis. Second, the neural network function library extensively uses template programming. When working with Vivado HLS, template programming allows the designer to specify a variety of useful constants to the HLS compiler, including both datatypes and network size. All of the layer sizes and weight/bias datatypes are specified using predefined typedefs in the example header.

2) **HLS Synthesis**: Next, the floating point data types of Step 1 are then converted into fixed point data types. Vivado provides a C++ class (ap_fixed) that simplifies the use of fixed point numbers. The "real world" value of the fixed point number may be used interchangeably with floating point; but the rounding, conversion, overflow, and all arithmetic/multiplication operations are handled by the HLS compiler. The designer will switch data types to fixed point numbers, compare results to simulation, and then adjust the fixed point data type format as required to meet the test vector. The process repeats until the fixed point results agree with the floating point results. 

    After the neural network is implemented in fixed point, HLS synthesis may run to create HDL code. The HLS directives in the neural network library have been optimized to provide an acceptable tradeoff of resources versus throughput results for a few network sizes-- if necessary, edit HLS directives to improve resource usage or algorithm throughput. 

3) **RFNoC HDL Compute Engine and Testbench**: After HDL is generated, the resulting module is inserted into an RFNoC CE. The RFNoC CE attaches to the RFNoC crossbar, which provides data routing throughout the rest of the FPGA and to the procesor. The neural network user logic does not typically contain the same number of inputs as outputs; therefore, simple mode of the axi_wrapper block must be set to zero, and the user connects the input and output axi-stream buses to the nnet_vector_wrapper. The nnet_vector_wrapper is a lightweight wrapping block that simply frames the data according to the neural network size from HLS. 

    Once the RFNoC CE is created, the user testbench can be created to stimulate the CE. In the provided examples, the test stimulus is chosen to be exactly the same as the HLS testbench data; using the same test stimulus provides a comforting knowledge that the synthesized FPGA code is functionally equivalent to the simulated C++ code. In the testbench folder, run the simulation to validate the CE functionality. Refer to the included readme files for simulation instructions. The RFNoC CE is now ready to be inserted into an FPGA image.

4) **Hardware Integration**: Finally, the CE is built into an FPGA image using the typical RFNoC image building workflow. At this point, the FPGA image can be programmed to the Ettus hardware of choice. The newly-created RFNoC block does not require custom C++ drivers, but it does need several xml definitions to be used in GRC. The rfnoc-hls-neuralnet module provides some basic examples of how to interface with the generated block. Inside rfnoc-hls-neuralnet/grc, the xml file identifies the block interface to GRC; this can be found in fpgannet_exmodrec.xml. Once the GRC xml file is ready, the RFNoC block can be inserted into a GRC flowgraph for user input and output, and then run locally or on the E300 series embedded devices. 

## Examples

In rfnoc-neuralnet/examples, there are a few pre-generated example RFNoC blocks that have been developed and tested against the E310 hardware platform. 

* ex_1layer: Simple one layer network for classifying grayscale letters A-J  (*tested on E310*)
* ex_modrec: A five layer fully-connected network for modulation recognition with expert feature input (*tested on E310*)
* ex_2layer: Larger two-layer network for classifying grayscale letters A-J (*HLS simulation ONLY*)
* ex_iqconv: A large and far more involved network for performing modulation recognition on raw RF (*HLS simulation ONLY*)

The first two examples, ex_1layer and ex_modrec, have been developed through the full HLS workflow and tested on an E310 using basic file input/input to confirm functionality. Note these blocks have not (yet) been tested with streaming RF data in order to perform unit tests in a constrained environment. A brief instruction on these examples are detailed below:  

#### noc_block_ex1layer

The neural network in this RFNoC block is modeled after the [first Tensorflow example](https://github.com/tensorflow/tensorflow/tree/master/tensorflow/examples/udacity) (specifically, 2_fullyconnected.ipynb) in the online [Udacity course on Deep Learning](https://www.udacity.com/course/deep-learning--ud730). This is a relatively simple one-layer fully-connected network, which I put together from the example notebook, then saved the pretrained weights and biases. The network is designed to accept 28 x 28 grayscale images from the "not_MNIST" dataset and to classify into 10 categories representing letters A-J.

**Running HLS Testbench:** To run the HLS example, follow the readme in rfnoc-neuralnet/rfnoc/hls/ex_1layer. When the project is open, run the C Simulation and C Synthesis to observe synthesized results.

**Running RFNoC Testbench:** Inside rfnoc-neuralnet/rfnoc/testbenches/noc_block_ex1layer_tb, run "make xsim_hls". The testbench should run and pass.

**Running GRC Flowgraph:** Open and compile rfnoc-neuralnet/examples/nnet_example_1layer.grc. To run on the E310, transfer the necessary cross-compiled prefix, the generated python flowgraph, and the dataset mnist_data_input.s16 to the E310. The mnist_data_input.s16 file contains a single image which is equivalent to the test data used in the HLS and RFNoC testbenches, and can be used as a regression-check on the E310. On the E310, run the generated python file. This should create a new file output "mnist_rfnoc_output.s16"-- you should be able to verify the contents of mnist_rfnoc_output.s16 exactly match the RFNoC testbench output. 

Optionally, in the nnet_example_1layer.grc flowgraph, enable the file source for ex1_binary_validation_data.s16. This contains all the validation data from the Udacity tensorflow course. I've saved this data on [my google drive](https://drive.google.com/open?id=0B4jB60EiIiLPLU95X3Jja280M0E) in order to avoid putting it on the git repo. Move the validation data to the E310 and run the python flowgraph pointing to the larger validation set.

#### noc_block_modrec

The neural network in this RFNoC block is modeled on modulation recognition work using expert "moment based" features [from grcon 2016](https://pubs.gnuradio.org/index.php/grcon/article/view/7). The idea is that certain features will be calculated on a segment of RF data and fed into the multi-layer network. This network requires 5 layers of essentially 40x40 fully-connected layers. Coincidentally, there are 10x input features and 10x output classifications.

**Running HLS Testbench:** To run the HLS example, follow the readme in rfnoc-neuralnet/rfnoc/hls/ex_modrec. When the project is open, run the C Simulation and C Synthesis to observe synthesized results.

**Running RFNoC Testbench:** Inside rfnoc-neuralnet/rfnoc/testbenches/noc_block_modrec_tb, run "make xsim_hls". The testbench should run and pass.

**Running GRC Flowgraph:** Open and compile rfnoc-neuralnet/examples/nnet_example_modrec.grc. To run on the E310, transfer the necessary cross-compiled prefix, the generated python flowgraph, and the dataset modrec_data_input.s16 to the E310. The modrec_data_input.s16 file contains a single set of 10 features which is equivalent to the test data used in the HLS and RFNoC testbenches, and can be used as a regression-check on the E310. On the E310, run the generated python file. This should create a new file output "modrec_rfnoc_output.s16"-- you should be able to verify the contents of modrec_rfnoc_output.s16 matches the RFNoC testbench output. 

Optionally, in the nnet_example_modrec.grc flowgraph, enable the file source for ex_modrec_validation_data.s16. This contains a larger set of 2000 modrec feature sets. I've saved this data on [my google drive](https://drive.google.com/open?id=0B4jB60EiIiLPLU95X3Jja280M0E) in order to avoid putting it on the git repo. Move the validation data to the E310 and run the python flowgraph pointing to the larger validation set. 

