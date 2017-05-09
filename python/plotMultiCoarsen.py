#!/usr/bin/env python

import os
import subprocess as sp
from shutil import move
import numpy as np
import matplotlib.pyplot as plt
import scipy.sparse as sps


def adj_plotter(dataFile,plotName,ms):
                
        # Load and plot
        f=open(dataFile)
        line0=f.readline()
        line1=f.readline()
        line2=f.readline()
        line3=f.readline()
        
        irow = [int(x) for x in line1.split()];
        vals = [float(x) for x in line2.split()];
        pcol = [int(x) for x in line3.split()];


        
        a = sps.csc_matrix( (vals,irow,pcol) )
        offDiagNNz = 0; 
        n = len(pcol)-1
        NNz=pcol[n]
        for i in xrange(0,n/2-1):
        	for j in xrange(pcol[i],pcol[i+1]):
        		if (irow[j] > (n/2.0-1)):
        			offDiagNNz += 1; 

        
        plt.figure(figsize=(15,15))
        mh=plt.spy(a,markersize=ms)
        plt.spy(a.transpose(),markersize=ms)
        plt.xlabel(("Total nnz = %d,     Off-Diagonal Blocks nnz = %d" % (NNz,2*offDiagNNz)),fontsize=32)

        
        #plt.show()
        plt.savefig(plotName,bbox_inches='tight',dpi=100)

#Data dir
dataDir="../results/multi-coarsen-01-procs"
figDir="../figs/multi-coarsen-01-procs"

# Make the plots
dataFileList = [("%s/adj_fb.dat" % dataDir),
                ("%s/adj_c1.dat" % dataDir),
                ("%s/adj_c2.dat" % dataDir),
                ("%s/adj_c2_reorder.dat" % dataDir),
                ("%s/adj_c1_reorder.dat" % dataDir),
                ("%s/adj_fb_reorder.dat" % dataDir)];
figNameList = [("%s/adj_fb.png" % figDir),
                ("%s/adj_c1.png" % figDir),
                ("%s/adj_c2.png" % figDir),
                ("%s/adj_c2_reorder.png" % figDir),
                ("%s/adj_c1_reorder.png" % figDir),
                ("%s/adj_fb_reorder.png" % figDir)];


ms=np.array([5.5,6.5,9,9,6.5,5.5])

for i in range(0,len(dataFileList)):
        adj_plotter(dataFileList[i],figNameList[i],ms[i])

        
