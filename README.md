# rfnoc-machinelearn

Open-source components for implementing neural networks in FPGA fabric. 

- Uses the RF Network-on-Chip ([RFNOC](https://www.ettus.com/sdr-software/detail/rf-network-on-chip)) framework for the software/FPGA IO interfaces. 
- Uses [Vivado HLS](https://www.xilinx.com/products/design-tools/vivado/integration/esl-design.html) to generate custom HDL.

Generally designed for RF processing, though the HLS portion may be more generally applicable to generating other neural nets.

## Goals

A neural network cannot be provided as a “one-size-fits-all” solution; the designer’s neural network architecture is a major driver of performance, and the variety of possible architectures is far too large to provide a generalized FPGA solution. Any two neural networks will have dramatically different architectures, including different flavors of convolutional or fully-connected layers, different activation functions, etc. This paradigm essentially reduces the impact of any pre-generated FPGA-based neural network designed fora specific network architecture.

Using a software-only approach, changing the neural net architecture is trivial. However, in hardware, FPGA fabric cannot be arbitrarily reconfigured on the fly; the desired architecture must be synthesized and implemented ahead of time for the optimal tradeoff of resource usage, compute time, and power. Therefore, the true strength of an FPGA-based neural network architecture would be the ability for the designer to regenerate a resource-efficient FPGA implementation in a short amount of time without needing to reinvent the wheel. Based on these assertions, the goals for the RFNoC Neural Network project are as follows:

* Develop an HLS library of common neural network functions
  * The library will include options such as different layers, activation functions, convolutional layers, etc.
  * The neural network functions can be combined in a conceptually similar process to using tensorflow. Using the C syntax in HLS, designers will instantiate the desired network architecture, test their implementation using HLS tools, and synthesize to HDL.

* Provide the FPGA architecture to wrap generated HDL code into an RFNoC compute engine (CE).
  * Use the AXI Stream interface to send samples to the CE

* Provide a software interface. The software will interact with the RFNoC CE to:
  * Program the neural net weights/biases at run time
  * Interface with the AXI Stream inputs and outputs
  * Provide a gnuradio-companion (GRC) interface

* Develop a demonstration application using the HLS + RFNoC + GRC workflow.
