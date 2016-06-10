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
y = pylab.loadtxt("ke_tuned.dat")


# setup the figure
fig = plt.figure()

# plot data
plt.plot(y[:,0],y[:,1],'k')

# set limits
axPlot = plt.subplot(111)
axPlot.set_xlim(-.001, 0.9)

# add titles, labels, and legend
plt.title('Kinetic energy of the rotating box in tuned adaptive scheme', fontdict=font)
plt.xlabel('Time', fontdict=font)
plt.ylabel('Kinetic Energy', fontdict=font)
plt.legend(loc=4, shadow=True)

# show the plot
#plt.show()

# save the plot
plt.savefig('adaptive-tuned.png')

