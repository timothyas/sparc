HELLO:=$(echo hello)
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
	echo ${BOOSTLIB}
	@echo "Available make targets:"
	@echo "  install   : build main program in /src/"
	@echo "  check	   : build and run test unit test suite in /test/unit"
	@echo "  coverage  : build tests w/ coverage option, run lcov, and generate html in /test/unit/lcov_html"
	@echo "  doc	   : build documentation (doxygen page, and writeup)" 
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

coverage:
	$(MAKE) -C ./test/unit clobber
	$(MAKE) -C ./test/unit CFLAGS="-O0 -g -Wall --coverage" LDFLAGS="${LDFLAGS} --coverage"
	$(MAKE) -C ./test/unit check 
	./include/lcov/bin/lcov -b ./test/unit --directory ./test/unit --directory ./src/ -no-external -c  -o coverage.info
	./include/lcov/bin/genhtml coverage.info -o lcov_html/
	mv coverage.info test/unit/
	mv lcov_html test/unit/

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
	-cd doc/doxygen && rm -rf html && rm -rf latex
	-cd ./test/system/ && rm -rf Sol.png
clean: 
	-$(MAKE) -C ./test/regression/ clean
	-$(MAKE) -C ./src/ clean
	-$(MAKE) -C ./doc/writeup clean
	

doc:
	$(MAKE) -C ./doc/writeup/
	cd doc/doxygen/ && doxygen v2f.dox



