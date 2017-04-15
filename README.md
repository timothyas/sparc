# spiC #

Sparse Pagerank Implementation in C++

# Work flow #

1. Load edge matrix

2. (sparse storage scheme?)

3. Coarsen graph

	a. Maximal Independent Set
	
        b. Graph coloring

	c. Edge matching 
	
4. Spectral bisection - reduce off diagonals for sparse matvec

5. Solve Mx = b 

# Package requirements #

We might use Eigen... tbd