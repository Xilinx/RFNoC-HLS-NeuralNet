#
# Copyright 2015-2017 Ettus Research
#
include $(TOOLS_DIR)/make/viv_hls_ip_builder.mak

# Set the IP Directory
FPGANNET_HLS_DIR := $(OOT_DIR)/hls

# Add HLS IP to build
include $(FPGANNET_HLS_DIR)/ex_1layer/Makefile.inc
include $(FPGANNET_HLS_DIR)/ex_modrec/Makefile.inc

hls_ip: $(HLS_IP_BUILD_TARGETS)

.PHONY: hls_ip
