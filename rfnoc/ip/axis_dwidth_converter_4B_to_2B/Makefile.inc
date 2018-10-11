#
# Copyright 2015 Ettus Research
#

include $(TOOLS_DIR)/make/viv_ip_builder.mak

LIB_IP_AXIS_DWIDTH_4B_TO_2B_SRCS = $(IP_BUILD_DIR)/axis_dwidth_converter_4B_to_2B/axis_dwidth_converter_4B_to_2B.xci

LIB_IP_AXIS_DWIDTH_4B_TO_2B_OUTS = $(addprefix $(IP_BUILD_DIR)/axis_dwidth_converter_4B_to_2B/, \
axis_dwidth_converter_4B_to_2B.xci.out \
synth/axis_dwidth_converter_4B_to_2B.vhd \
)

$(LIB_IP_AXIS_DWIDTH_4B_TO_2B_SRCS) $(LIB_IP_AXIS_DWIDTH_4B_TO_2B_OUTS) : $(RFNOC_NEURALNET_DIR)/ip/axis_dwidth_converter_4B_to_2B/axis_dwidth_converter_4B_to_2B.xci
	$(call BUILD_VIVADO_IP,axis_dwidth_converter_4B_to_2B,$(ARCH),$(PART_ID),$(RFNOC_NEURALNET_DIR)/ip,$(IP_BUILD_DIR),0)
