# Running list of TODOs...

Still working through learning HLS: 

[ ] Play with optimization tradeoffs for complex FIR (area, multipliers, throughput, etc)


Start HLS IP for neural nets:

[ ] New HLS IP with complex inputs, but real FIR filter only. (... basic building block of neural nets)
[ ] Add activation layer
[ ] Test demonstration neural net using 16 bit representation- compare results vs reference
[ ] Run demo in HLS
[ ] Simulate in noc_block + resource estimate

Also note: Not sure if autoencoders are the appropriate first step. Either way, need to get the basics running,  which includes...

* FIR + activation function
* Multiple layers (including convolutional??)
* How to deal with convolutional vs non-convolutional?
* Input/outputs for noc_block interface


More advanced stuff:

[ ] Where to go next??