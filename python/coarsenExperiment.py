#!/usr/bin/env python

import os
import subprocess as sp
from shutil import move
import numpy as np
import matplotlib.pyplot as plt
import scipy.sparse as sps


nProcs=1
fbData="../data/facebook_combined.txt"
outName="Results.dat"
saveNameBase = "../results/adjacency_"
if not os.path.isdir('../results'):
        os.makedirs('../results')


# Lets get spicy
for n in range(0,10):

        print ""
        print " -- Start ", n, " level coarsening"
        print ""

        # Run executable
        arg1= "-f %s" % fbData
        arg2= '-c %d' % n
        arg3= "-p %d" % nProcs
        exeLine= '%s %s %s %s' % ('../src/spiC',arg1,arg2,arg3)

        sp.call(exeLine,shell=True)

        ## Save output based on number of coarsens
        if n<10:
                saveName="%s%d0%s.dat" % (saveNameBase,n,"c")     
                plotName="%s%d0%s.png" % (saveNameBase,n,"c")
        else:
                saveName="%s%d%s.dat" % (saveNameBase,n,"c")
                plotName="%s%d%s.png" % (saveNameBase,n,"c")
        
        print ""
        print " -- Copying results to: ", saveName
        move(outName,saveName)

        # Plot it up
        a = np.loadtxt(saveName)
        a = a + np.transpose(a)
        N = len(a)
        offDiagNnz = np.sum(np.sum(a[0:N/2,N/2:N]))

        a = sps.csr_matrix(a)

        plt.figure(figsize=(15,15))
        mh=plt.spy(a,markersize=1.5)

        legStr = 'NNz: %d \n Off Diag NNz: %d' % (N,offDiagNnz)
        plt.legend((legStr,))
        
       # plt.show()
        plt.savefig(plotName,bbox_inches='tight',dpi=100)
        
        




        
