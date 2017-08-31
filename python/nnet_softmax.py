"""
Embedded Python Blocks:

Each time this file is saved, GRC will instantiate the first class it finds
to get ports and parameters of your block. The arguments to __init__  will
be the parameters. All of them are required to have default values!
"""

import numpy as np
from gnuradio import gr

def softmax(x):
    """Compute softmax values for each sets of scores in x."""
    e_x = np.exp(x - np.max(x))
    return e_x / e_x.sum()

class nnet_softmax(gr.sync_block):  # other base classes are basic_block, decim_block, interp_block
    """Embedded Python Block example - a simple multiply const"""

    def __init__(self, vector_size=10): 
        gr.sync_block.__init__(
            self,
            name='nnet_softmax',
            in_sig=[np.float32],
            out_sig=[np.float32]
        )
        self.vector_size = vector_size
        self.set_output_multiple(vector_size)

    def work(self, input_items, output_items):
        if len(input_items[0]) >= self.vector_size:
            output_items[0][0:10] = softmax(input_items[0][:10])
            return self.vector_size
        else:
            return 0