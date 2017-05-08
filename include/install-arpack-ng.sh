#!/bin/sh
BLAS_LIB=`ldconfig -p | grep blas | tail -1 | cut -f 2 -d'>' | cut -f 2 -d' '`
LAPACK_LIB=`ldconfig -p | grep lapack | tail -1 | cut -f 2 -d'>' | cut -f 2 -d' '`
cd arpackpp
set -ex
mkdir -p external
cd external
git clone https://github.com/opencollab/arpack-ng.git
mkdir arpack-ng-build
cd arpack-ng-build
cmake -D BLAS_goto2_LIBRARY=$BLAS_LIB -D LAPACK_goto2_LIBRARY=$LAPACK_LIB ../arpack-ng
#/usr/lib64/../external/libopenblas.a ../arpack-ng
make
cd ../
ln -s arpack-ng-build/libarpack.a ./
cd ../
