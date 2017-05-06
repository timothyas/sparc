#!/usr/bin/env python

import os
import subprocess as sp
from shutil import move
import numpy as np
import matplotlib.pyplot as plt
import scipy.sparse as sps


nCoarse=20
nProcs=1
fbData="../data/facebook_combined.txt"
outName="Results.dat"
outEdgeName="edgeList.txt"
edgeNameBase = "../results/multi-coarsen/edge_"
saveNameBase = "../results/multi-coarsen/adjacency_"
plotNameBase = "../figs/multi-coarsen/adjacency_"

if not os.path.isdir('../results'):
        os.makedirs('../results')
if not os.path.isdir('../results/multi-coarsen'):
        os.makedirs('../results/multi-coarsen')
if not os.path.isdir('../figs'):
        os.makedirs('../figs')
if not os.path.isdir('../figs/multi-coarsen'):
        os.makedirs('../figs/multi-coarsen')


# Lets get spicy
odNNz = np.zeros(nCoarse)

for n in range(0,nCoarse):

        ## Prepare output & check existence
        if n<10:
                edgeName="%s0%d%s.txt" % (edgeNameBase,n,"c")
                saveName="%s0%d%s.dat" % (saveNameBase,n,"c")     
                plotName="%s0%d%s.png" % (plotNameBase,n,"c")
        else:
                edgeName="%s%d%s.txt" % (edgeNameBase,n,"c")
                saveName="%s%d%s.dat" % (saveNameBase,n,"c")
                plotName="%s%d%s.png" % (plotNameBase,n,"c")

        if not os.path.isfile(saveName):
                print ""
                print " -- Start ", n, " level coarsening"
                print ""

                # Run executable
                arg1= "-f %s" % fbData
                arg2= '-c %d' % n
                arg3= "-p %d" % nProcs
                exeLine= '%s %s %s %s' % ('../src/spiC',arg1,arg2,arg3)

                sp.call(exeLine,shell=True)
                
                print ""
                print " -- Moving results to: ", saveName
                move(outName,saveName)
                move(outEdgeName,edgeName)

        # Load and plot
        a = np.loadtxt(saveName)
        a = a + np.transpose(a)
        N = len(a)
        odNNz[n] = np.sum(np.sum(a[0:N/2,N/2:N]))

        a = sps.csr_matrix(a)

        plt.figure(figsize=(15,15))
        mh=plt.spy(a,markersize=1.5)

        legStr = 'NNz: %d \n Off Diag NNz: %d' % (N,odNNz[n])
        plt.legend((legStr,))
        
       # plt.show()
        plt.savefig(plotName,bbox_inches='tight',dpi=100)
        
# Plot off diagonal nnz
plt.figure(figsize=(15,10))
plt.plot(odNNz)
plt.xlabel('Levels Coarsened')
plt.ylabel('Off Diagonal Black NNz')
plt.savefig("../figs/multi-coarsen/offDiagNNz.png",bbox_inches='tight',dpi=100)
