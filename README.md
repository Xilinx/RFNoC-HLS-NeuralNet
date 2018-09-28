# rfnoc-hls-neuralnet

EJ Kreinar (ejkreinar@gmail.com)

With support from: Darek Kawamoto, Dan CaJacob, Hawkeye 360 (http://www.he360.com/)

## What is it?

Open-source components for implementing neural networks in FPGA fabric. 

- Uses [Vivado HLS](https://www.xilinx.com/products/design-tools/vivado/integration/esl-design.html) to generate custom HDL for the neural-network.
- Uses the RF Network-on-Chip ([RFNOC](https://www.ettus.com/sdr-software/detail/rf-network-on-chip)) framework for the software/FPGA IO interfaces around the HDL outputs. 

Designed primarily for RF processing research and development, though the HLS neural network library may be generally applicable to other types of neural nets (image processing, for example).

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

The RFNoC neural network library (rfnoc-neuralnet) provides an RFNoC OOT module for efficiently deploying a trained neural network to an FPGA. Typically, neural networks are designed, trained, and executed on a conventional processor, often with GPU acceleration. But for embedded devices which may need to process data at multiple-MHz sample rates, the computational requirements can be overwhelming for an embedded processor where no GPU is available, creating a tempting opportunity for FPGA acceleration.

The neural network HLS library ([rfnoc/hls/nnet_lib](rfnoc/hls)) provides pre-optimized compiler directives that modify the physical hardware implementation for larger or smaller networks as appropriate to achieve an acceptable tradeoff of throughput vs resources. For specialized applications, HLS directives may be edited to specifically influence the synthesized HDL architecture. 

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

Next, clone the rfnoc-hls-neuralnet repo into the prefix/src directory: `git clone https://github.com/Xilinx/RFNoC-HLS-NeuralNet.git rfnoc-hls-neuralnet`

Finally, rfnoc-hls-neuralnet is *almost* compatible with uhd-fpga tagged at v3.13.0.0. I have a small "out of tree" patch that adds data files to the HLS makefile process (hosted here: https://github.com/ejk43/fpga/tree/v3.13.0.0-hls-data)

To apply patches:

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

1) **HLS Floating-point Simulation**: HLS development begins in the HLS folder: [rfnoc/hls](rfnoc/hls). In the initial stage, the developer first implements and simulates the desired neural network using floating point datatypes in C++ using the provided code from rfnoc-hls-neuralnet. The general goal in this step is to identically replicate the performance of the selected neural network algorithm in the HLS framework.

    This code,  for example,  is pulled straight from [ex_modrec.cpp](rfnoc/hls/ex_modrec/ex_modrec.cpp):

    ```C++
    // LAYER 1
    hls::stream<layer1_t> logits1, hidden1;
    nnet::compute_layer<layer0_t, layer1_t, weight_t, bias_t, accum_t, N_LAYER_IN, N_LAYER_1>
                       (data_trunc, logits1, w1, b1);
    nnet::relu6<layer1_t, layer1_t, N_LAYER_1>(logits1, hidden1);

    // LAYER 2
    hls::stream<layer2_t> logits2, hidden2;
    nnet::compute_layer<layer1_t, layer2_t, weight_t, bias_t, accum_t, N_LAYER_1, N_LAYER_2>
                       (hidden1, logits2, w2, b2);
    nnet::relu6<layer2_t, layer2_t, N_LAYER_2>(logits2, hidden2);

    [...]

    // LAYER 5
    nnet::compute_layer<layer4_t, layer5_t, weight_t, bias_t, accum5_t, N_LAYER_4, N_LAYER_5>
                       (hidden4, res, w5, b5);
    ```
    This may look busy at first, but the neural network code is relatively straightforward after looking past the template declarations (dense layer -> relu -> dense layer -> relu -> ... -> dense layer). There are two major differences between the HLS-optimized code and a typical C or C++ implementation.

    First, the input/output interface to each function uses Vivado's hls_stream object, which provides a FIFO buffer that helps developers program C++ code that is naturally conducive to FPGA synthesis.

    Second, the neural network function library extensively uses template programming, which appears clunky but is required in order for the Vivado HLS to determine array sizes and data types at compile-time. When working with Vivado HLS, template programming allows the designer to specify a variety of useful constants to the HLS compiler, including both datatypes and network size. In the code snippet above, the "\*\_t" commands all refer to typedefs that can be changed from floating point to fixed point in the file header. In the typical use case, the designer will instantiate the required network parameters using this template interface to the HLS neural network library, which allows the functions to generate HDL code for a wide variety of network sizes and datatypes.

2) **HLS Synthesis**: Next, the floating point data types of Step 1 are then converted into fixed point data types. Vivado provides a C++ class (ap_fixed) that simplifies the use of fixed point numbers. The "real world" value of the fixed point number may be used interchangeably with floating point; but the rounding, conversion, overflow, and all arithmetic/multiplication operations are handled by the HLS compiler. The designer will switch data types to fixed point numbers (typically performed via typedefs in the respective \*.h files), compare results to simulation, and then adjust the fixed point data type format as required to meet the test vector. The process repeats until the fixed point results agree with the floating point results.

    After the neural network is implemented in fixed point, HLS synthesis may run to create HDL code. Of course, synthesis may be ran with floating point data types-- but the resource performance is often 2-4x higher than expected. The HLS directives in the neural network library have been optimized to provide an acceptable tradeoff of resources versus throughput results for a few network sizes-- if necessary, edit HLS directives to improve resource usage or algorithm throughput.

3) **RFNoC HDL Compute Engine and Testbench**: After HDL is generated, the resulting module is inserted into an RFNoC CE. This operation is performed in the RFNoC [fpga-src](rfnoc/fpga-src) and [testbench](rfnoc/testbenches) folders. The RFNoC CE attaches to the RFNoC crossbar, which provides data routing throughout the rest of the FPGA and to the procesor. The neural network user logic does not typically contain the same number of inputs as outputs; therefore, simple mode of the axi_wrapper block must be set to zero, and the user connects the input and output axi-stream buses to the nnet_vector_wrapper. The nnet_vector_wrapper is a lightweight wrapping block that simply frames the data according to the neural network size from HLS. The examples provided in both noc_block_ex1layer.v and noc_block_modrec.v can be followed to demonstrate how to correctly wrap the generated output for either ap_fifo output ports or ais-stream output ports.

    Once the RFNoC CE is created, the user testbench can be created to stimulate the CE. In the provided examples, the test stimulus is chosen to be exactly the same as the HLS testbench data; using the same test stimulus provides a comforting knowledge that the synthesized FPGA code is functionally equivalent to the simulated C++ code. In the testbench folder, run the simulation to validate the CE functionality. Refer to the included readme files for simulation instructions. The RFNoC CE is now ready to be inserted into an FPGA image.

4) **Hardware Integration**: Finally, the CE is built into an FPGA image using the typical RFNoC image building workflow. At this point, the FPGA image can be programmed to the Ettus hardware of choice. The newly-created RFNoC block does not require custom C++ drivers, but it does need several xml definitions to be used in GRC. The rfnoc-hls-neuralnet module provides some basic examples of how to interface with the generated block. The following items are required:

    * [UHD RFNoC block definition](rfnoc/blocks)
    * [GRC block definition](grc)
    * [GRC flowgraph](examples)

    Once the xml definitions are ready, the RFNoC block can be inserted into a GRC flowgraph for user input and output, and then run locally for the X300 or remotely on the E300 series embedded devices.

## Examples

In rfnoc-neuralnet/examples, there are a few pre-generated example RFNoC blocks that have been developed and tested against the E310 hardware platform.

* [ex_1layer](rfnoc/hls/ex_1layer): Simple one layer network for classifying grayscale letters A-J  (*tested on E310*)
* [ex_modrec](rfnoc/hls/ex_modrec): A five layer fully-connected network for modulation recognition with expert feature input (*tested on E310*)
* [ex_2layer](rfnoc/hls/ex_2layer): Larger two-layer network for classifying grayscale letters A-J (*HLS simulation ONLY*)
* [ex_iqconv](rfnoc/hls/ex_iqconv): A large and far more involved network for performing modulation recognition on raw RF (*HLS simulation ONLY*)

The first two examples, ex_1layer and ex_modrec, have been developed through the full HLS workflow and tested on an E310 using basic file input/input to confirm functionality at a unit test level. Note these blocks have not (yet) been tested with streaming RF data. A brief instruction on these examples are detailed below:

### noc_block_ex1layer

The neural network in this RFNoC block is modeled after the [first Tensorflow example](https://github.com/tensorflow/tensorflow/tree/master/tensorflow/examples/udacity) (specifically, 2_fullyconnected.ipynb) in the online [Udacity course on Deep Learning](https://www.udacity.com/course/deep-learning--ud730). This is a relatively simple one-layer fully-connected network, which I put together from the example notebook, then saved the pretrained weights and biases. The network is designed to accept 28 x 28 grayscale images from the "not_MNIST" dataset and to classify into 10 categories representing letters A-J.

**Running HLS Testbench:** To run the HLS example, follow the readme in rfnoc-neuralnet/rfnoc/hls/ex_1layer. When the project is open, run the C Simulation and C Synthesis to observe synthesized results.

**Running RFNoC Testbench:** Inside rfnoc-neuralnet/rfnoc/testbenches/noc_block_ex1layer_tb, run "make xsim_hls". The testbench should run and pass.

**Running GRC Flowgraph:** Open and compile rfnoc-neuralnet/examples/nnet_example_1layer.grc. To run on the E310, transfer the necessary cross-compiled prefix, the generated python flowgraph, and the dataset mnist_data_input.s16 to the E310. The mnist_data_input.s16 file contains a single image which is equivalent to the test data used in the HLS and RFNoC testbenches, and can be used as a regression-check on the E310. On the E310, run the generated python file. This should create a new file output "mnist_rfnoc_output.s16"-- you should be able to verify the contents of mnist_rfnoc_output.s16 exactly match the RFNoC testbench output. 

Optionally, in the nnet_example_1layer.grc flowgraph, enable the file source for ex1_binary_validation_data.s16. This contains all the validation data from the Udacity tensorflow course. I've saved this data on [my google drive](https://drive.google.com/open?id=0B4jB60EiIiLPLU95X3Jja280M0E) in order to avoid putting it on the git repo. Move the validation data to the E310 and run the python flowgraph pointing to the larger validation set.

### noc_block_modrec

The neural network in this RFNoC block is modeled on modulation recognition work using expert "moment based" features [from grcon 2016](https://pubs.gnuradio.org/index.php/grcon/article/view/7). The idea is that certain features will be calculated on a segment of RF data and fed into the multi-layer network. This network requires 5 layers of essentially 40x40 fully-connected layers. Coincidentally, there are 10x input features and 10x output classifications.

**Running HLS Testbench:** To run the HLS example, follow the readme in rfnoc-neuralnet/rfnoc/hls/ex_modrec. When the project is open, run the C Simulation and C Synthesis to observe synthesized results.

**Running RFNoC Testbench:** Inside rfnoc-neuralnet/rfnoc/testbenches/noc_block_modrec_tb, run "make xsim_hls". The testbench should run and pass.

**Running GRC Flowgraph:** Open and compile rfnoc-neuralnet/examples/nnet_example_modrec.grc. To run on the E310, transfer the necessary cross-compiled prefix, the generated python flowgraph, and the dataset modrec_data_input.s16 to the E310. The modrec_data_input.s16 file contains a single set of 10 features which is equivalent to the test data used in the HLS and RFNoC testbenches, and can be used as a regression-check on the E310. On the E310, run the generated python file. This should create a new file output "modrec_rfnoc_output.s16"-- you should be able to verify the contents of modrec_rfnoc_output.s16 matches the RFNoC testbench output. 

Optionally, in the nnet_example_modrec.grc flowgraph, enable the file source for ex_modrec_validation_data.s16. This contains a larger set of 2000 modrec feature sets. I've saved this data on [my google drive](https://drive.google.com/open?id=0B4jB60EiIiLPLU95X3Jja280M0E) in order to avoid putting it on the git repo. Move the validation data to the E310 and run the python flowgraph pointing to the larger validation set.


## Follow-Up

There remain, of course, many improvements I would like to see to this module. Here's an assorted list in order of decreasing priority, for public reference and for my own memory...

* **Additional neural network layer types**: While the fully-connected layers and convolutional layers cover many types of neural networks for RF processing, a few more building blocks could be helpful as part of the library, specifically 1) 2D convolutional layers, 2) recurrent neural networks, and 3) softmax operation (while usually only needed on the output of a network during training, it's nevertheless a common structure often used in software).
* **Work out 32-bit vs 16-bit datatypes**: There's some gymnastics going on with the data type sizing on the FPGA, and I hit two odd stumbling blocks in this area... 1) As is obvious in the GRC flowgraphs, the RFNoC architecture is primarily designed for 32 bit sample streaming (16-bits I,  16-bits Q). It's less clear how exactly to handle sequential 16-bit chunks of data correctly. I found *something* that works in the flowgraph examples, but this appears to use a good deal more processing horsepower on the embedded device than should be needed. 2) HLS failed at generating outputs with 32-bit sizes when developing the ex_modrec example. This was a real struggle-- HLS said that everything passed, yet the noc_block_modrec_tv.sv simulation consistently failed to get valid data out of the HLS generated IP block when I used 32-bit outputs. Anyway, the data type issues need to be worked out.
* **Test with live streaming data**: The current repo demonstrates example RFNoC neural network blocks on hardware as file-read/write unit tests, but does not take the additional step of integrating with live RF data. Live integration at a high data rate would be an excellent demonstration application for the rfnoc-hls-neuralnet library.
* **Programmable weights**: Programmable neural network weights were originally on the development roadmap, but were eventually deemed too ambitious for the proof-of-concept architecture presented here. In order to maintain simplicity, weights are required to be fixed in the current implementation; however, programmable weights could become a higher priority improvement depending on demand. 
    Notionally, a reprogrammable implementation would require the HLS library to include an empty memory bank of the correct size and expose a programming interface; software would then require a C++ driver to program weights upon initialization. While it's a plausible approach, it adds a significant amount complexity to the FPGA and software wrappers.  
* **Improve weight storage**: The BRAM usage of Vivado HLS generated code tends to be higher than expected. This is due to HLS's BRAM packing algorithms and that HLS apparently uses BRAM18s while BRAM36s may also be available. It is likely that most neural networks synthesized for the RFNoC architecture will be memory-limited rather than computationally limited. 
    A few options here: The BRAM could be moved to outside of the HLS code and into hand-coded HDL, which may be slightly more efficient, though more difficult to implement. As a different (related) option, a hardware platform might have FPGA access to onboard RAM, which could be used to store large amounts of neural network coefficients. Unfortunately, RFNoC does not currently provide an interface to on-chip RAM; this improvement would necessarily manifest as an application-specific task.
* **Provide alternate neural network architectures**: Researchers have shown that binarized neural networks can be very efficient on FPGAs while maintaining high performance levels. A binarized neural network could efficiently store weights and perform multiplications (which evaluate into basic binary operations for one bit values), but this manifests as a slightly different HLS structure than assumed here. 
