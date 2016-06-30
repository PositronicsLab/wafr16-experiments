# this program computes a histogram of the step size data for the chain
# nearing the  

import os,sys
import pylab
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.lines as lines

# setup font
font =  {'family' : 'sans-serif',
         'color'  : 'black',
         'weight' : 'normal',
         'size'   : 16,
        }

# read in the l20 file 
l20 = pylab.loadtxt("distances-l20.dat")

# setup the figure
fig = plt.figure()

# plot data
n, bins, patches = plt.hist(l20[:,1], 50, normed=0, facecolor='green', alpha=0.75) 

# add titles, labels, and legend
plt.title('Histogram of CA steps for the chain scenario', fontdict=font)
plt.xlabel('Conservative step (maximum 0.01)', fontdict=font)
plt.ylabel('Count', fontdict=font)
plt.grid(True)

# show the plot
#plt.show()

# save the plot
plt.savefig('hist.png')

