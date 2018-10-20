# rfnoc-hls-neuralnet

EJ Kreinar (ejkreinar@gmail.com)

With support from: Hawkeye 360 (http://www.he360.com/) and the hls4ml project (https://hls-fpga-machine-learning.github.io/hls4ml/)

## What is it?

Open-source components for implementating neural network inference in FPGA fabric for RF signal processing.

- Uses [Vivado HLS](https://www.xilinx.com/products/design-tools/vivado/integration/esl-design.html) to generate custom HDL for the neural-network.
- Uses [hls4ml](https://github.com/hls-fpga-machine-learning/hls4ml) to create the neural-network C++ code used with Vivado HLS -- this allows developers to write their neural network using typical python tools (keras, pytorch), then generate the C++ code that corresponds with the particular network implementation.
- Uses the RF Network-on-Chip ([RFNOC](https://www.ettus.com/sdr-software/detail/rf-network-on-chip)) framework for the software/FPGA IO interfaces around the HDL outputs.

Designed primarily for RF processing research and development, though the HLS neural network library may be generally applicable to other types of neural nets (image processing, for example).

The neural network primitives provided by the hls4ml project are under development and increasing. A non-comprehensive list of supported network constructs includes:
 * Fully connected layer
 * Convolutional layer
 * Maxpool
 * Batch norm
 * Various activation functions
   * relu
   * leaky relu
   * sigmoid
   * tanh
   * and more
 * Softmax

For additional information and documentation on hls4ml, see the [project website](https://hls-fpga-machine-learning.github.io/hls4ml/).

## Summary

The RFNoC neural network library (rfnoc-hls-neuralnet) provides an RFNoC OOT module for efficiently deploying a trained neural network to an FPGA. Typically, neural networks are designed, trained, and executed on a conventional processor, often with GPU acceleration. But for embedded devices which may need to process data at multiple-MHz sample rates, the computational requirements can be overwhelming for an embedded processor where no GPU is available, creating a tempting opportunity for FPGA acceleration.

The hls4ml library allows developers to prototype and train neural networks in python (keras or pytorth), then *export* this network into an FPGA implementation to perform optimized inference. After hls4ml generates the C++ project template for a network, developers are able to use high-level, layer-specific parameters to fine tune the FPGA implementation in HLS; This allows for rapid evaluation of resource vs throughput tradeoffs and experimentation to find the "optimal" instantiation of a particular network on a per-application basis. The output of the hls4ml workflow is a Xilinx HLS IP core that can be used across a variety of FPGA workflows, including on AWS, SDAccel, or in this case, inside a signal processing application using RFNoC.

RFNoC provides a convenient input/output interface between hardware and software that is compatible with gnuradio. Ideally, the neural network designer will be able to deploy neural networks and evaluate resource vs throughput tradeoffs without needing to develop and maintain the repetitive “glue code” in FPGA and software. Presented examples demonstrate various use-cases in a simulation environment and on the E310, including image classification and modulation recognition, using both fully-connected and convolutional layers.

![HLS NeuralNet Block Diagram](/images/block-diagram.png)

## Goals

* Develop an HLS library of common neural network functions
  * The library includes options such as different layers, activation functions, convolutional layers, etc.
  * The neural network functions can be combined in a conceptually similar process to using tensorflow. Using the C syntax in HLS, designers will instantiate the desired network architecture, test their implementation using HLS tools, and synthesize to HDL.

* Provide the FPGA architecture to wrap generated HDL code into an RFNoC compute engine (CE).
  * Use the AXI Stream interface to send samples to the CE

* Provide a software interface. The software will interact with the RFNoC CE to:
  * Interface with the AXI Stream inputs and outputs
  * Provide a gnuradio-companion (GRC) interface
  * Program the neural net weights/biases at run time (not currently supported, but a nice-to-have future feature)

* Develop a demonstration application using the HLS + RFNoC + GRC workflow.

# Instructions

[updated 9/27/2018 for uhd-fpga v3.13.0.0 and Vivado 2017.4]

First, make sure you have an rfnoc gnuradio prefix installed according to Ettus's [Getting Started with RFNoC guide](https://kb.ettus.com/Getting_Started_with_RFNoC_Development).

Next, clone the rfnoc-hls-neuralnet repo into the prefix/src directory (make sure to use the recursive flag to also clone hls4ml): `git clone --recursive https://github.com/Xilinx/RFNoC-HLS-NeuralNet.git rfnoc-hls-neuralnet`

Finally, rfnoc-hls-neuralnet is *almost* compatible with uhd-fpga tagged at v3.13.0.0. I have a small "out of tree" patch that adds data files to the HLS makefile process (hosted here: https://github.com/ejk43/fpga/tree/v3.13.0.0-hls-data)

To use the correct version of UHD:

1. cd into your uhd-fpga repo
2. `git remote add ejk https://github.com/ejk43/fpga.git`
3. `git fetch ejk v3.13.0.0-hls-data`
4. `git checkout v3.13.0.0-hls-data `  (alternatively, cherry-pick the relevant update, or merge into your working repo, etc)

From here, you are set up to do the following operations:

1. In the [HLS folder](rfnoc/hls), generate an HLS project for one of the example or test projects. Re-synthesize HLS outputs to evaluate resource usage and throughput. Follow the readme for instructions.
2. In the [testbench](rfnoc/testbenches) folder, run `make xsim_hls` in the example of your choice to stimulate the full neural network RFNoC compute engine
3. In uhd-fpga/usrp3/top/e300 (or uhd-fpga/usrp3/top/x300), add the rfnoc-hls-neuralnet repo to your Makefile.OOT.inc file,  then generate an RFNOC image for a device
4. After an FPGA image is created, compile a [GRC flowgraph example](examples) and run to interact with the FPGA and perform basic unit tests of the network operation.

When generating a new FPGA image, the following lines need to be in the Makefile.OOT.inc (where rfnoc-hls-neuralnet is the name of the cloned repo):

```
OOT_DIR = $(BASE_DIR)/../../../rfnoc-hls-neuralnet/rfnoc
include $(OOT_DIR)/Makefile.inc
```

This syntax should be automatically generated correctly if you use the uhd_image_builder or uhd_image_builder_gui scripts, pointing to the rfnoc-hls-neuralnet as an Include file.


## Workflow

The HLS-RFNoC workflow is a bit involved, yet worthwhile.

![HLS NeuralNet Workflow Diagram](/images/workflow-diagram.png)

0) **Prototype and Train a Neural Network**: "Step 0" of this workflow, not shown on the diagram above, is to first prototype and train a neural network using typical, industry-standard python-based tools such as keras or pytorch. When the network is ready to target FPGAs for optimized inference, export the network from python and run the hls4ml C++ generation tool to create a starting point for running Xilinx HLS. Note that the IO architecture of RFNoC is designed to use the "serial mode" option of hls4ml -- which means that features are clocked into and out of the HLS IP core one-by-one (rather than presented to the IP core all at once).

1) **HLS Simulation**: HLS development lives in the HLS folder: [rfnoc/hls](rfnoc/hls). After generating the network using hls4ml,  the next goal is to identically replicate the performance of the neural network algorithm in the HLS framework. This can usually be accomplished using the C++ defaults from Step 0; however, the user may want to simulate a known test scenario to confirm successful translation from python to C++. This can be done using floating point values by changing typedefs in the parameters.h file.

2) **HLS Synthesis**: Next, the HLS is then synthesized to evaluate resource usage. Vivado provides a C++ class (ap_fixed) that simplifies the use of fixed point numbers. The "real world" value of the fixed point number may be used interchangeably with floating point; but the rounding, conversion, overflow, and all arithmetic/multiplication operations are handled by the HLS compiler. After the neural network is implemented in fixed point, HLS synthesis may run to create HDL code.

    The default parameters specified by hls4ml are usually "okay", but depending on the particular network architecture, there may be more optimized solutions, or the user may choose to perform resource vs throughput tradeoffs for a particular network. If necessary or desired, edit the HLS parameters in `parameters.h` to improve resource usage or algorithm throughput. A good example of a small change to more optimally use FPGA resources is in the "keras_3layer" example, which changes a few of the `reuse_factor` parameters from 1 to 2; this change decreases DSP usage by about 30 DSPs without impacting either the fabric usage or the core throughput.

3) **RFNoC HDL Compute Engine and Testbench**: After HDL is generated, the resulting module is inserted into an RFNoC CE. This operation is performed in the RFNoC [fpga-src](rfnoc/fpga-src) and [testbench](rfnoc/testbenches) folders. The RFNoC CE attaches to the RFNoC crossbar, which provides data routing throughout the rest of the FPGA and to the procesor. The neural network user logic does not typically contain the same number of inputs as outputs; therefore, simple mode of the axi_wrapper block must be set to zero, and the user connects the input and output axi-stream buses to the nnet_vector_wrapper. The nnet_vector_wrapper is a lightweight wrapping block that can simply perform byte packing or unpacking as desired. The examples provided in fpga-src can be followed to demonstrate how to correctly wrap the generated output for the generated HLS IP core.

    Once the RFNoC CE is created, the user testbench can be created to stimulate the CE. In the provided examples, the test stimulus duplicates the HLS testbench data (all 0s input and output equal to the HLS simulation output). The developer may want a more useful set of stimulus and verification data. In the testbench folder, run the simulation to validate the CE functionality. Refer to the included readme files for simulation instructions. The RFNoC CE is now ready to be inserted into an FPGA image.

4) **Hardware Integration**: Finally, the CE is built into an FPGA image using the typical RFNoC image building workflow. At this point, the FPGA image can be programmed to the Ettus hardware of choice. The newly-created RFNoC block does not require custom C++ drivers, but it does need several xml definitions to be used in GRC. The rfnoc-hls-neuralnet module provides some basic examples of how to interface with the generated block. The following items are required:

    * [UHD RFNoC block definition](rfnoc/blocks)
    * [GRC block definition](grc)

Once the xml definitions are ready, the RFNoC block can be inserted into a GRC flowgraph for user input and output, and then run locally for the X300 or remotely on the E300 series embedded devices.

## Examples

In rfnoc-neuralnet/examples, there are a few pre-generated example RFNoC blocks that have been developed and tested against the E310 hardware platform.

* [keras_1layer](rfnoc/hls/keras_1layer): A small classifier network with one hidden layer.
* [keras_3layer](rfnoc/hls/keras_3layer): A slightly bigger classifier network with three hidden layers.

These updated examples are derived from examples provide in hls4ml, and have been exercised through HLS and FPGA simulation; however they have not been fully exercised on a hardware platform (yet). Use at your own risk! And let me know if this is of interest and I can take a look if there's any issues.

**Running HLS Testbenches:** In the [hls](rfnoc/hls) folder, follow to readme to run vivado_hls against the example projects. Use the HLS GUI if desired to examine resource outputs, throughput limitations, etc.

**Running RFNoC Testbenches:** In the [testbench](rfnoc/testbenches) folder, follow the readme to run xsim simulations with HLS generation. Dont forget to update uhd-fpga with the patch(es) for HLS builds!

## Follow-Up

There remain, of course, many improvements I would like to see to this module. Here's an assorted list in order of decreasing priority, for public reference and for my own memory...

* **Additional neural network layer types**: While hls4ml provides several tested layer types, including fully-connected layers and convolutional layers, it's still a work in progress and more layers are under development and coming soon (with more testing and coverage)
* **Test with live streaming data**: The current repo demonstrates example RFNoC neural network blocks on hardware as file-read/write unit tests, but does not take the additional step of integrating with live RF data. Live integration at a high data rate would be an excellent demonstration application for the rfnoc-hls-neuralnet library.
* **Programmable weights**: Programmable neural network weights were originally on the development roadmap, but were eventually deemed too ambitious for the proof-of-concept architecture presented here. In order to maintain simplicity, weights are required to be fixed in the current implementation; however, programmable weights could become a higher priority improvement depending on demand.
* **Improve weight storage**: The BRAM usage of Vivado HLS generated code tends to be higher than expected. This is due to HLS's BRAM packing algorithms and that HLS apparently uses BRAM18s while BRAM36s may also be available. It is likely that most neural networks synthesized for the RFNoC architecture will be memory-limited rather than computationally limited.
    A few options here: The BRAM could be moved to outside of the HLS code and into hand-coded HDL, which may be slightly more efficient, though more difficult to implement. As a different (related) option, a hardware platform might have FPGA access to onboard DRAM, which could be used to store large amounts of neural network coefficients. Unfortunately, RFNoC does not currently provide an interface to on-chip RAM; this improvement would necessarily manifest as an application-specific task.
* **Provide alternate neural network architectures**: Researchers have shown that binarized neural networks can be very efficient on FPGAs while maintaining high performance levels. A binarized neural network could efficiently store weights and perform multiplications (which evaluate into basic binary operations for one bit values), but this manifests as a slightly different HLS structure than assumed here.
