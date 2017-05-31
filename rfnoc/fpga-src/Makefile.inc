##################################################
# RFNoC Sources
##################################################

RFNOC_SRCS += $(addprefix $(RFNOC_NEURALNET_DIR)/fpga-src/, \
noc_block_ex1layer.v \
nnet_vector_wrapper.v \
packet_resizer_variable.v \
)
