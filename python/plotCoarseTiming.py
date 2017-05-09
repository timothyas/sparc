
import numpy as np
import matplotlib.pyplot as plt

figDir='../figs'

tot=np.array([241.4,166.7,86,72.3,85.7,114])
colorTime=np.array([236,161,79.6,65.3,76.15,93.4]) 
nprocs=np.array([1,4,16,32,64,68])

plt.figure(figsize=(15,10))
plt.semilogx( nprocs, colorTime, "*-",label='Coloring' )
plt.semilogx( nprocs, tot,"*-",label='Total Coarsening' )
plt.xlabel('Num. Threads',fontsize=24)
plt.ylabel('Time (s)',fontsize=24)
plt.legend(loc='best')
plt.savefig(("%s/color_timing.png" % figDir),bbox_inches='tight',dpi=100)


