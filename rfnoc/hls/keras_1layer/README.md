# Keras_1layer Example

Uses the network defined inside hls4ml: KERAS_1layer

To recreate code generation:

1. Install hls4ml python dependencies (or create a virtualenv/conda env, etc). See hls4ml for instructions
2. Within this directory, run: `python ../../hls4ml/keras-to-hls/keras-to-hls.py -c keras-1layer-config.yml`

Step 2 will use the local config file (keras-1layer-config.yml) to regenerate the C++ code to synthesize the 1-layer example.

For Vivado 2017.4:
  - Remove the `config_array_partition` directive inside the build_prj.tcl

To build the project with HLS:

1. Run build_prj.tcl: `vivado_hls -f build_prj.tcl`
2. If desired, open the HLS project: `vivado_hls -p keras_1layer_prj`
