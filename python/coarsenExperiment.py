#!/usr/bin/env python

import os
import subprocess as sp
from shutil import move
import numpy as np
import matplotlib.pyplot as plt
import scipy.sparse as sps


# --- Run time options & output files
nCoarse=10
nProcs=12
fbData="../data/facebook_combined.txt"
outName="Results.dat"
outEdgeName="edgeList.txt"

# -- Result directory name
if nProcs > 9:
        runDir = "multi-coarsen-%d-procs" % nProcs
else:
        runDir = "multi-coarsen-0%d-procs" % nProcs

# --- Set up dirs
resultsDir = "../results"
figsDir = "../figs"
expOutput = "%s/%s" % (resultsDir,runDir)
expFigs = "%s/%s" % (figsDir,runDir)
edgeNameBase = "%s/edge_" % expOutput
saveNameBase = "%s/adjacency_" % expOutput
plotNameBase = "%s/adjacency_" % expFigs

if not os.path.isdir(resultsDir):
        os.makedirs(resultsDir)
if not os.path.isdir(figsDir):
        os.makedirs(figsDir)
if not os.path.isdir(expOutput):
        os.makedirs(expOutput)
if not os.path.isdir(expFigs):
        os.makedirs(expFigs)


# --- Lets get spicy
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
                #move(outEdgeName,edgeName)

        # Load and plot
        a = np.loadtxt(saveName)
        a = a + np.transpose(a)
        N = len(a)
        odNNz[n] = np.sum(np.sum(a[0:N/2,N/2:N]))

        a = sps.csr_matrix(a)

        plt.figure(figsize=(15,15))
        mh=plt.spy(a,markersize=1.5)

        legStr = 'N/2: %d \n Off Diag NNz: %d' % (N/2,odNNz[n])
        plt.legend((legStr,))
        
       # plt.show()
        plt.savefig(plotName,bbox_inches='tight',dpi=100)
        
# Plot off diagonal nnz
odnnzFig="%s/offDiagNNz.png" % expFigs
plt.figure(figsize=(15,10))
plt.plot(odNNz)
plt.xlabel('Levels Coarsened')
plt.ylabel('Off Diagonal Black NNz')
plt.savefig(odnnzFig,bbox_inches='tight',dpi=100)
