import sys
import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) > 1:
    names = sys.argv[1:]
else:
    fname = "out.dat"

names.sort()
dat = np.genfromtxt(names[0], delimiter=1, dtype='|S1')

for name in names[1:]:
    dat1 = np.genfromtxt(name, delimiter=1, dtype='|S1')
    dat = np.concatenate((dat,dat1), axis=1)

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
plt.gca().spines["top"].set_visible(False)
plt.gca().spines["bottom"].set_visible(False)
plt.gca().spines["left"].set_visible(False)
plt.gca().spines["right"].set_visible(False)

plt.savefig("im_cells.png")
plt.show()
