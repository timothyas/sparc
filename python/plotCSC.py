#!/usr/bin/env python

import os
import subprocess as sp
from shutil import move
import numpy as np
import matplotlib.pyplot as plt
import scipy.sparse as sps



# Load and plot
f=open("Results.dat")
line1=f.readline()
line2=f.readline()
line3=f.readline()

irow = [int(x) for x in line1.split()];
vals = [float(x) for x in line2.split()];
pcol = [int(x) for x in line3.split()];

a = csc_matrix( (val,irow,pcol) )

plt.figure(figsize=(15,15))
mh=plt.spy(a,markersize=1.5)

plt.show()
plt.savefig(plotName,bbox_inches='tight',dpi=100)
        
# Plot off diagonal nnz
odnnzFig="%s/offDiagNNz.png" % expFigs
plt.figure(figsize=(15,10))
plt.plot(odNNz)
plt.xlabel('Levels Coarsened')
plt.ylabel('Off Diagonal Black NNz')
plt.savefig(odnnzFig,bbox_inches='tight',dpi=100)
