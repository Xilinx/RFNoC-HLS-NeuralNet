##################################################
# RFNoC Sources
##################################################

RFNOC_SRCS += $(addprefix $(RFNOC_NEURALNET_DIR)/fpga-src/, \
noc_block_keras1layer.v \
nnet_vector_wrapper.v \
)
