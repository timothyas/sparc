#!/usr/bin/env python

import os
import subprocess as sp
from shutil import move
import numpy as np
import matplotlib.pyplot as plt
import scipy.sparse as sps


def adj_plotter(dataFile,plotName):
                
        # Load and plot
        f=open(dataFile)
        line0=f.readline()
        line1=f.readline()
        line2=f.readline()
        line3=f.readline()
        
        irow = [int(x) for x in line1.split()];
        vals = [float(x) for x in line2.split()];
        pcol = [int(x) for x in line3.split()];

        print "%d %d " %(irow[0],pcol[1])
        
        N=len(pcol)-1
        NNz=pcol[N]
        
        a = sps.csc_matrix( (vals,irow,pcol) )
        
        plt.figure(figsize=(15,15))
        mh=plt.spy(a,markersize=1.5)
        
        #plt.show()
        plt.savefig(plotName,bbox_inches='tight',dpi=100)

#Data dir
dataDir="../results/multi-coarsen-12-procs"
figDir="../figs/multi-coarsen-12-procs"

# Make the plots
dataFileList = [("%s/adj_fb.dat" % dataDir),
                ("%s/adj_c1.dat" % dataDir),
                ("%s/adj_c2.dat" % dataDir),
                ("%s/adj_c2_reorder.dat" % dataDir),
                ("%s/adj_c1_reorder.dat" % dataDir),
                ("%s/adj_fb_reorder.dat" % dataDir)];
figNameList = [("%s/adj_fb.dat" % figDir),
                ("%s/adj_c1.dat" % figDir),
                ("%s/adj_c2.dat" % figDir),
                ("%s/adj_c2_reorder.dat" % figDir),
                ("%s/adj_c1_reorder.dat" % figDir),
                ("%s/adj_fb_reorder.dat" % figDir)];

for i in range(0,len(dataFileList)):
        adj_plotter(dataFileList[i],figNameList[i])

        
