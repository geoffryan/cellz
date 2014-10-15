import sys
import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) > 1:
    fname = sys.argv[1]
else:
    fname = "out.dat"

dat = np.genfromtxt(fname, delimiter=1, dtype='|S1')
dat[dat==" "] = 0
dat[dat=="*"] = 1

fig = plt.figure(figsize=(9,9))
plt.matshow(dat.astype(float), fig.number, cmap=plt.cm.binary)
plt.tick_params(bottom='off',
                top='off',
                left='off',
                right='off',
                labelbottom='off',
                labeltop='off',
                labelleft='off',
                labelright='off')

plt.savefig("im_cells.png")
plt.show()
