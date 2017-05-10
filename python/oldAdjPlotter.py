#!/usr/bin/env python

import os
import subprocess as sp
from shutil import move
import numpy as np
import matplotlib.pyplot as plt
import scipy.sparse as sps


# --- Run time options & output files
nExps=10
nCoarse=10
nProcs=12
fbData="../data/facebook_combined.txt"
outName="Results.dat"
outEdgeName="edgeList.txt"
timingName="timing.txt"

# -- Result directory name
runDir = "fb-timing"

# --- Set up dirs
resultsDir = "../results"
figsDir = "../figs"
expOutput = "%s/%s" % (resultsDir,runDir)
expFigs = "%s/%s" % (figsDir,runDir)

if not os.path.isdir(resultsDir):
        os.makedirs(resultsDir)
if not os.path.isdir(figsDir):
        os.makedirs(figsDir)
if not os.path.isdir(expOutput):
        os.makedirs(expOutput)
if not os.path.isdir(expFigs):
        os.makedirs(expFigs)

# --- Lets get spicy
procList = np.array([1,2,4,12])
odNNz = np.zeros((len(procList),nCoarse,nExps))
colorTiming = np.zeros((len(procList),nCoarse,nExps))
mxmTiming = np.zeros((len(procList),nCoarse,nExps))
parentTiming = np.zeros((len(procList),nCoarse,nExps))
sbTiming = np.zeros((len(procList),nCoarse,nExps))
mvTiming = np.zeros((len(procList),nCoarse,nExps))

print "Heads up: running on 1, 2, 4, and 12 cores"

for ip in range(0,nProcs):
        for n in range(0,nCoarse):

                print ""
                print " -- Start ", n, " level coarsening"
                print ""

                for i in range(0,nExps):

                        ## Prepare output & check existence
                        print " Experiment %02d/%02d" % (i+1,nExps+1)

                        # Run executable
                        arg1= "-f %s" % fbData
                        arg2= '-c %d' % n
                        arg3= "-p %d" % procList[ip]
                        exeLine= '%s %s %s %s' % ('../src/spiC',arg1,arg2,arg3)

                        sp.call(exeLine,shell=True)

                        # Read results and fill containers
                        a=np.loadtxt(outName);
                        a = a + np.transpose(a)
                        N = len(a)
                        odNNz[ip,n,i] = np.sum(np.sum(a[0:N/2,N/2:N]))

                        t=np.loadtxt(timingName);
                        colorTiming[ip,n,i] = t[0]
                        mxmTiming[ip,n,i] = t[1]
                        parentTiming[ip,n,i] = t[2]
                        sbTiming[ip,n,i] = t[3]
                        mvTiming[ip,n,i] = t[4]
                        

                
# plot and Save by nprocs
for ip in range(0,nProcs):

        cMean=np.mean(colorTiming[ip,:,:],axis=1)       
        mxmMean=np.mean(mxmTiming[ip,:,:],axis=1)       
        pMean=np.mean(parentTiming[ip,:,:],axis=1)       
        sbMean=np.mean(sbTiming[ip,:,:],axis=1)       
        mvMean=np.mean(mvTiming[ip,:,:],axis=1)       
        odMean=np.mean(odNNz[ip,:,:],axis=1)

        cstd=np.std(colorTiming[ip,:,:],axis=1)       
        mxmstd=np.std(mxmTiming[ip,:,:],axis=1)       
        pstd=np.std(parentTiming[ip,:,:],axis=1)       
        sbstd=np.std(sbTiming[ip,:,:],axis=1)       
        mvstd=np.std(mvTiming[ip,:,:],axis=1)       
        odstd=np.std(odNNz[ip,:,:],axis=1)

        #Timing plot
        x=range(1,nCoarse+1)
        plt.figure(figsize=(15,10))
        plt.errorbar(x,cMean,cstd)
        plt.xlabel('Levels coarsened')
        plt.ylabel('Coloring timing')
        fname='%s/colorTiminig_%02dp.png' % (expFigs,procList[ip])
        plt.savefig(fname,bbox_inches='tight',dpi=100)
        plt.clf()

        plt.errorbar(x,mxmMean,mxmstd)
        plt.xlabel('Levels coarsened')
        plt.ylabel('Maximal Matching timing')
        fname='%s/mxmTiminig_%02dp.png' % (expFigs,procList[ip])
        plt.savefig(fname,bbox_inches='tight',dpi=100)
        plt.clf()

        plt.errorbar(x,pMean,pstd)
        plt.xlabel('Levels coarsened')
        plt.ylabel('Parent graph creation timing')
        fname='%s/parentTiminig_%02dp.png' % (expFigs,procList[ip])
        plt.savefig(fname,bbox_inches='tight',dpi=100)
        plt.clf()

        plt.errorbar(x,sbMean,sbstd)
        plt.xlabel('Levels coarsened')
        plt.ylabel('SB timing')
        fname='%s/sbTiminig_%02dp.png' % (expFigs,procList[ip])
        plt.savefig(fname,bbox_inches='tight',dpi=100)
        plt.clf()

        plt.errorbar(x,mvMean,mvstd)
        plt.xlabel('Levels coarsened')
        plt.ylabel('MatVec timing')
        fname='%s/mvTiminig_%02dp.png' % (expFigs,procList[ip])
        plt.savefig(fname,bbox_inches='tight',dpi=100)
        plt.clf()

        plt.errorbar(x,odMean,odstd)
        plt.xlabel('Levels coarsened')
        plt.ylabel('Off Diagonal NNz')
        fname='%s/odnnz_%02dp.png' % (expFigs,procList[ip])
        plt.savefig(fname,bbox_inches='tight',dpi=100)
        plt.clf()

        colorName='color-%02dp.txt' % procList[ip]
        mxmName='mxm-%02dp.txt' % procList[ip]
        parentName='parent-%02dp.txt' % procList[ip]
        sbName='sb-%02dp.txt' % procList[ip]
        mvName='mv-%02dp.txt' % procList[ip]
        odName='od-%02dp.txt' % procList[ip]

        np.savetxt(colorName,colorTiming[ip,:,:],fmt='%.5f')
        np.savetxt(mxmName,mxmTiming[ip,:,:],fmt='%.5f')
        np.savetxt(parentName,parentTiming[ip,:,:],fmt='%.5f')
        np.savetxt(sbName,sbTiming[ip,:,:],fmt='%.5f')
        np.savetxt(mvName,mvTiming[ip,:,:],fmt='%.5f')
        np.savetxt(odName,odNNz[ip,:,:],fmt='%d')


