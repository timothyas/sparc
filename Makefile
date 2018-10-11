#Change the following lines to the root path of BOOST and ARPACK
#BOOST_DIR=
SPARC_DIR= ${PWD}
ARPACK_DIR=${SPARC_DIR}/include/arpackpp/

INC	:= -I${SPARC_DIR}/include/ -I${ARPACK_DIR}/include  -I${ARPACK_DIR}/include/../examples/matrices  -I${ARPACK_DIR}/include/../examples/areig/  -I${ARPACK_DIR}/include/../examples/matprod -I${BOOST_DIR}/incldue/

#Add the location of blas and lapack here.
LDFLAGS  := -L${ARPACK_DIR}/external -L${BOOST_DIR}/lib/ 
LDLIBS     :=  -larpack -lsuperlu -lgfortran  -llapack  -lblas -lboost_program_options

EXEC := sparc
EXECDIR := ../

# Export variables so check, install, and coverage can all use libraries
export LDFLAGS
export INC
export LDLIBS
export LIB
export EXEC
export EXECDIR

info:

	@echo "Available make targets:"
	@echo "  all       : build main program"
	@echo "  check	    : build and run test suite"
	@echo "  doc       : build documentation (doxygen page)"
	@echo "  dep       : build dependencies"

all:
	$(MAKE) -C ./src/  

check: 
	-cp ./src/graph.cpp ./test/regression
	-cp ./src/coarsen.cpp ./test/regression
	-cp ./src/spectralBisection.cpp ./test/regression
	-cp ./src/matrixOperations.cpp ./test/regression
	$(MAKE)	-C ./test/regression
	$(MAKE) -C ./test/regression check
	-rm ./test/regression/graph.cpp
	-rm ./test/regression/coarsen.cpp
	-rm ./test/regression/spectralBisection.cpp
	-rm ./test/regression/matrixOperations.cpp
	$(MAKE) -C ./test/regression clean

dep:
	-cd include/ && ./install-dep.sh
	@echo "All dependencies installed"
	
love:
	@echo "not war?"

.PHONY: clobber, clean, doc
clobber: clean 
	-$(MAKE) -C ./src/ clobber
	-$(MAKE) -C ./test/regression/ clobber
	-$(MAKE) -C ./doc/writeup clean
clean: 
	-$(MAKE) -C ./test/regression/ clean
	-$(MAKE) -C ./src/ clean
doc:
	cd doc/doxygen/ && doxygen sparc.dox
