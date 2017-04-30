# Just a little helper function to save data in a vector format
# Works for 1D or 2D data. Appends vector size to filename
# Use the read functions in hls/nnet_lib/nnet_helpers.h

# Example: 
#   import numpy as np
#   from save_data_file import savedata
#   myvec = np.arange(0, 10, 0.3)
#   savedata(myvec, 'myvec_example')

datafolder = 'data'
def savedata(data, prefix, folder='data'):
    import os
    if not os.path.isdir(folder):
        os.mkdir(folder)
    shape = data.shape
    if len(shape) == 2:
        basename = prefix + "_%ix%i" % (shape[0], shape[1]) + ".dat"
    elif len(shape) == 1:
        basename = prefix + "_%ix1" % (shape[0]) + ".dat"
    else:
        print("Cannot save vector with more than 2 dimensions.")
        return ""
    filename = os.path.join(datafolder, basename)
    fid = open(filename, 'w')
    data.tofile(fid, sep="\n", format="%s")
    fid.close()
    return filename

