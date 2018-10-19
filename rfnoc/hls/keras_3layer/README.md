# Keras_3layer Example

Uses the network defined inside hls4ml: KERAS_3layer

To recreate code generation:

1. Install hls4ml python dependencies (or create a virtualenv/conda env, etc). See hls4ml for instructions
2. Within this directory, run: `python ../../hls4ml/keras-to-hls/keras-to-hls.py -c keras-3layer-config.yml`

Step 2 will use the local config file (keras-3layer-config.yml) to regenerate the C++ code to synthesize the 1-layer example.

To build the project with HLS:

1. Run build_prj.tcl: `vivado_hls -f build_prj.tcl`
2. If desired, open the HLS project: `vivado_hls -p keras_1layer_prj`
