# SPARC #

Sparse PAgeRank in C++

# Package requirements #

1. ARPACK
2. SuperLU
3. Boost

# Installation and Usage #

SPARC can be build on either Stampede 2 (KNL) or on the ICES machines. To build on the ICES Machines 
first load gcc and boost module, 

>> module load gcc boost

Then issue a `make dep` to build all external dependencies: 

>> make dep

Run the test suit by issuing a `make check` command. 

>> make check 

All unit tests should pass if built correctly. Lastly, issue a `make install` to build the code in the src/
directory. 

>> make install

To run the code, cd into the src/ directory and run `./sparc` to display the help message. A typical run might be: 

>> ./sparc -f ../data/facebook_combined.txt -c 4 -p 2 

This will run the facebook data with 4 levels of coarsening on 2 threads. The data directory contains more test cases. 

To build the code on Stampede 2, first load the gcc, mkl, and boost modules. 

>> module load gcc mkl boost

The follow the same steps above but use MakefileKNL, i.e,

>> make -f MakefileKNL [options]

