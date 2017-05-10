#!/usr/bin/env python

import os
import subprocess as sp
from shutil import move
import numpy as np
import matplotlib.pyplot as plt
import scipy.sparse as sps


def adj_plotter(readname,writename):
        # Read results and fill containers
        a=np.loadtxt(readname)
        a=sps.csc_matrix(a)
        
        
        plt.figure(figsize=(15,15))
        mh=plt.spy(a,markersize=1.5)
        plt.spy(a.transpose(),markersize=1.5)
        plt.savefig(writename,bbox_inches='tight',dpi=100)


# --- Directory
oldResults="../results/multi-coarsen-12-procs/old"
oldFigs="../figs/multi-coarsen-12-procs/old"

adj_plotter( ("%s/adjacency_02c.dat" % oldResults), ("%s/adjacency_02c.png" % oldFigs) )
adj_plotter( ("%s/adjacency_09c.dat" % oldResults), ("%s/adjacency_09c.png" % oldFigs) )
