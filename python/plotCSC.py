#!/usr/bin/python

import os
import subprocess as sp
from shutil import move
import numpy as np
import matplotlib.pyplot as plt
import scipy.sparse as sps



# Load and plot
f=open("../output/Results.dat")
f.readline()
line1=f.readline()
line2=f.readline()
line3=f.readline()

irow = [int(x) for x in line1.split()];
vals = [float(x) for x in line2.split()];
pcol = [int(x) for x in line3.split()];

offDiagNNZ = 0; 
n = len(pcol)-1
for i in xrange(0,n/2-1):
	for j in xrange(pcol[i],pcol[i+1]):
		if (irow[j] > (n/2.0-1)):
			offDiagNNZ += 1; 

print "Off Diag nnz = " + str(2*offDiagNNZ)
a = sps.csc_matrix( (vals,irow,pcol) )

plt.figure(figsize=(15,15))
mh=plt.spy(a,markersize=0.1)

plt.show()
#plotName = "DBLP_SBC4.png"
plt.xlabel('Total nnz = ' + str((len(vals))) + ',     Off-Diagonal Blocks nnz = ' + str(2*offDiagNNZ))
plt.title('DBLP Spectral Bisection after 4 levels of Coarsening')
plt.savefig(plotName,bbox_inches='tight',dpi=100)
        

# Plot off diagonal nnz
#odnnzFig="%s/offDiagNNz.png" % expFigs
#plt.figure(figsize=(15,10))
#plt.plot(odNNz)
#plt.xlabel('Levels Coarsened')
#plt.ylabel('Off Diagonal Black NNz')
#plt.savefig(odnnzFig,bbox_inches='tight',dpi=100)
