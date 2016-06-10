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

# load data
y = pylab.loadtxt("ke.dat")


# setup the figure
fig = plt.figure()

# plot data
plt.plot(y[:,0],y[:,1],'k')

# set limits
axPlot = plt.subplot(111)
axPlot.set_xlim(-.001, 4)

# add titles, labels, and legend
plt.title('Kinetic energy of the spinning box in adaptive scheme', fontdict=font)
plt.xlabel('Time', fontdict=font)
plt.ylabel('Kinetic Energy', fontdict=font)
plt.legend(loc=4, shadow=True)

# show the plot
#plt.show()

# save the plot
plt.savefig('adaptive.png')

