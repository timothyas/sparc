#!/usr/bin/env python

import os
import subprocess as sp
from shutil import move
import numpy as np
import matplotlib.pyplot as plt
import scipy.sparse as sps

def read_odnnz(filename):
        f=open(filename)
        f.readline()
        line1=f.readline()
        line2=f.readline()
        line3=f.readline()
        
        irow = [int(x) for x in line1.split()];
        vals = [float(x) for x in line2.split()];
        pcol = [int(x) for x in line3.split()];

        N=len(pcol)-1
        odNNz=0
        for i in xrange(0,N/2-1):
        	for j in xrange(pcol[i],pcol[i+1]):
        		if (irow[j] > (N/2.0-1)):
        			odNNz += 1; 
        return odNNz

def read_sb_timing(filename):
        t=np.loadtxt(filename)
        if len(t.shape)>1:
                sbTiming = t[3][0]
        else:
                sbTiming = t[3]
        return sbTiming

# --- Run time options & output files
nCoarse=10
nProcs=12
fbData="../data/facebook_combined.txt"
outName="Results.dat"
outTime="timing.txt"

# -- Result directory name
if nProcs > 9:
        runDir = "odnnz-test" 
else:
        runDir = "odnnz-test"

# --- Set up dirs
resultsDir = "../results"
figsDir = "../figs"
expOutput = "%s/%s" % (resultsDir,runDir)
expFigs = "%s/%s" % (figsDir,runDir)
timingNameBase = "%s/time_" % expOutput
saveNameBase = "%s/adj_" % expOutput

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
sbTiming = np.zeros(nCoarse)

for n in range(0,nCoarse):

        ## Prepare output & check existence
        saveName="%s%02d%s.dat" % (saveNameBase,n,"c")
        timingName="%s%02d%s.dat" % (timingNameBase,n,"c")

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
                
                print " -- Moving results to: ", saveName
                move(outName,saveName)
                move(outTime,timingName)
        
        odNNz[n] = read_odnnz(saveName)
        sbTiming[n] = read_sb_timing(timingName)


# Grabbing some desirable results
niceDataDir="../results/multi-coarsen-12-procs/old"
for n in range(0,nCoarse):
        loadName="%s/adjacency_%02dc.dat" % (niceDataDir,n)
        a=np.loadtxt(loadName)
        a=a+a.transpose()
        N=len(a)
        odNNz[n] = np.sum(np.sum(a[N/2:N,0:N/2]))


        
# Plot off diagonal nnz
odnnzFig="%s/offDiagNNz.png" % expFigs
plt.figure(figsize=(15,10))
plt.plot(odNNz,label='Off Diagonal Block nnz',linewidth=2.5)
plt.xlabel('Levels Coarsened',fontsize=32)
plt.ylabel('Off Diagonal Block NNz',fontsize=32)
plt.savefig(odnnzFig,bbox_inches='tight',dpi=100)

# Plot Spectral Bisection timing
sbFig = "%s/sbTiming.png" % expFigs
plt.figure(figsize=(15,10))
plt.loglog(sbTiming,label='Spectral Bisection Timing',linewidth=2.5)
plt.xlabel('Levels Coarsened',fontsize=32)
plt.ylabel('Spectral Bisection Timing (s)',fontsize=32)
plt.savefig(sbFig,bbox_inches='tight',dpi=100)

