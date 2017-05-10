BOOSTLIB := $(shell env | grep BOOST | grep LIB | cut -f 2 -d=)
BOOSTINC := $(shell env | grep BOOST | grep INC | cut -f 2 -d=)
SPIC_DIR = ${PWD}
INC	:= -I${SPIC_DIR}/include/ -I${SPIC_DIR}/include/arpackpp/include  -I${SPIC_DIR}/include/arpackpp/include/../examples/matrices  -I${SPIC_DIR}/include/arpackpp/include/../examples/areig/  -I${SPIC_DIR}/include/arpackpp/include/../examples/matprod -I${BOOSTINC}

LDFLAGS  := -L${SPIC_DIR}/include/arpackpp/external -L${BOOSTLIB} 
LDLIBS     :=  -larpack -lsuperlu -lgfortran  -llapack  -lblas -lboost_program_options

# Export variables so check, install, and coverage can all use libraries
export LDFLAGS
export INC
export LDLIBS
export LIB

all: info

info:
	@echo "Available make targets:"
	@echo "  install   : build main program in /src/"
	@echo "  check	    : build and run test suite in /test/regression"
	@echo "  dep       : build dependencies"

install:
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
