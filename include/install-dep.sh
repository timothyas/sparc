#!/bin/bash 
cd ..
SPIC_DIR=`pwd`
cd ./include

if [ -d "arpackpp" ]; then
	echo Found arpackpp
else
	echo Installing arpackpp
	git clone https://github.com/m-reuter/arpackpp.git
fi

cd arpackpp/

#CHECK FOR ARPACK-NG
if [ -f "./external/libarpack.a" ]; then 
	echo Found arpack-ng.
else
	echo Installing arpack-ng
	./install-arpack-ng.sh
fi

if $(ldconfig -p | grep -q "[g]fortran"); then 
	GFORTRAN_LIB=`ldconfig -p | grep "gfortran" | cut -f 2 -d'>'`
	echo Found gfortran.
fi


##CHECK FOR BLAS AND GFORTRAN
if $(ldconfig -p | grep -q "[o]penblas"); then 
	OPENBLAS_LIB=`ldconfig -p | grep "openblas" | cut -f 2 -d'>'`
	echo Found openblas.
	if [ -f "./external/libsuperlu.a" ]; then
		echo "Found SuperLU."
	else
		export BLAS="SYSTEM" && ./install-superlu.sh
	fi
else
	echo Installing openblas: 
	./install-openblas.sh
	OPENBLAS_LIB=$SPIC_DIR/include/arpackpp/externals/libopenblas.a
	if [ -f "./external/libsuperlu.a" ]; then
		echo "Found SuperLU."
	else
		./install-superlu.sh
	fi
fi

if [ -d "./external/SuiteSparse" ]; then 
	echo Found SuiteSparse
else
	echo Installing SuiteSparse
	./install-suitesparse.sh
fi

#if ! [ -d "./build/examples" ]; then 
#	echo Building arpackpp
#	mkdir build
#	cd build
#	export GFORTRAN_LIB=$GFORTRAN_LIB
#	echo $OPENBLAS_LIB
#	echo $GFORTRAN_LIB
#	cmake -D SUPERLU=ON -D CHOLMOD=ON -D UMFPACK=ON -D BLAS_goto2_LIBRARY=$OPENBLAS_LIB GFORTRAN_goto2_LIBRARY=$GFORTRAN_LIB GFORTRAN_LIB=$GFORTRAN_LIB ../
#	make examples
#fi
#