#
# Copyright 2015-2017 Ettus Research
#
include $(TOOLS_DIR)/make/viv_hls_ip_builder.mak

# Add HLS IP to build
include $(OOT_DIR)/test_hls/Makefile.inc
include $(OOT_DIR)/fir_hls/Makefile.inc

hls_ip: $(HLS_IP_BUILD_TARGETS)

.PHONY: hls_ip
