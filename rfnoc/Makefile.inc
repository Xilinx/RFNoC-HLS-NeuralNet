# Specify all relevant Makefiles to include

RFNOC_NEURALNET_DIR := $(OOT_DIR)
include $(abspath $(RFNOC_NEURALNET_DIR)/fpga-src/Makefile.inc)
include $(abspath $(RFNOC_NEURALNET_DIR)/hls/Makefile.inc)
