#
# Copyright 2015 Ettus Research
#

include $(TOOLS_DIR)/make/viv_ip_builder.mak

LIB_IP_AXIS_DWIDTH_2B_TO_4B_SRCS = $(IP_BUILD_DIR)/axis_dwidth_converter_2B_to_4B/axis_dwidth_converter_2B_to_4B.xci

LIB_IP_AXIS_DWIDTH_2B_TO_4B_OUTS = $(addprefix $(IP_BUILD_DIR)/axis_dwidth_converter_2B_to_4B/, \
axis_dwidth_converter_2B_to_4B.xci.out \
synth/axis_dwidth_converter_2B_to_4B.v \
)

$(LIB_IP_AXIS_DWIDTH_2B_TO_4B_SRCS) $(LIB_IP_AXIS_DWIDTH_2B_TO_4B_OUTS) : $(RFNOC_NEURALNET_DIR)/ip/axis_dwidth_converter_2B_to_4B/axis_dwidth_converter_2B_to_4B.xci
	$(call BUILD_VIVADO_IP,axis_dwidth_converter_2B_to_4B,$(ARCH),$(PART_ID),$(RFNOC_NEURALNET_DIR)/ip,$(IP_BUILD_DIR),0)
