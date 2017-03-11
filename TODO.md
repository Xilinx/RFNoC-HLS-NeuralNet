# Running list of TODOs...

Still working through learning HLS: 

- [ ] Examine waveform trace output from HLS project
- [ ] Play with optimization tradeoffs for complex FIR (area, multipliers, throughput, etc)
- [ ] Larger complex FIR filter. Parameterize Ntaps effectively?
- [ ] Explore coefficient load process interface

Start HLS IP for neural nets:

- [ ] New HLS IP using real FIR filter only. (FIR filter is essentially the building block of neural nets)
- [ ] Add activation layer
- [ ] Test demonstration neural net using 16 bit representation. Compare results vs (floating pt) reference
- [ ] Run demo in HLS
- [ ] Simulate in noc_block + resource estimate

Also note: Not sure what's the appropriate first step. Autoencoder looks like a good way to test just a couple of layers in a somewhat useful demo. Either way, need to get the basics running, which includes...

* FIR + activation function
* Multiple layers (including convolutional??)
* How to deal with convolutional vs non-convolutional?
* Input/outputs for noc_block interface


More advanced stuff:

- [ ] Where to go next??
