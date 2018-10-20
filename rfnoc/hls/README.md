# HLS Code

The example HLS projects in this repo demonstrate the
workflow for generating new neural network IP using hls4ml.

Provided examples are:
  - keras_1layer: A small classifier network with one hidden layer.
  - keras_3layer: A slightly bigger classifier network with three hidden layers.

These examples are created using the hls4ml "example-keras-model-files"
that represent several pretrained networks used for testing the
hls4ml library.

## Implementing your own network

The recommended approach for implementing a new network is
to copy the general idea in these examples (except target
your own JSON and h5 files, of course).

This would look something like this:

1. Create and train your neural network using python, perhaps
using Keras or Pytorch.
2. Export the network into the json and h5 file representation
(follow instructions in hls4ml to do this export)
3. Fork this repo, or set up your own repo similar to this one.
4. Add a subfolder here with your network definition. Call hls4ml
to generate the C++ code (see examples), confirm operation, and
commit to your repo!
5. If desired, open HLS and observe resource usage, try tuning
parameters to get towards better resoure usage, etc.
6. Add relevant makefiles to your hls folder.

After creating the HLS code, create a new noc_block, testbench,
and xml definitions similar to the examples -- the formula ends
up pretty similar from this point onward.
