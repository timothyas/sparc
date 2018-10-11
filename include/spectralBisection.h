/**
 * \file
 * \author Gopal Yalla and Tim Smith
 * 
 * \brief Defines functions to perform spectral bisection.
 *
 * Spectral Bisection
 * ==================
 *
 * The main computation in solving the page rank problem reduces to a matrix vector multiplication. 
 * For large data sets, the problem can be solved more efficeitnly by performing the matvecs in parallel. However, any
 * part of the matvec operation cooresponding to data in the off-diagonal blocks of the matrix requires between node/processor 
 * communication. This is shown in the following figure where there are 4 processors in total.
 *
 * \htmlonly
 * <IMG SRC="matvec.png" width=700px height=300px align="middle"/>
 * \endhtmlonly
 *
 * Clearly the cost of the matvec will be proportional to the number of edges across processors. 
 * If the matrix is sparse, we can reduce the amount of communication by reordering the matrix before the matvec 
 * in a way that reduces the number of off diagonal entries. For relatively small data sets, spectral bisection can be used 
 * to determine this reordered. The steps of spectral bisection include
 *
 * -# Form the adjacency matrix of a graph G
 * -# Form the graph laplacian of G
 * -# Compute the second smallest eigenvalue of graph laplacian.
 * -# Sort nodes based on median of the second eigenvalue of the graph laplacian. 
 *
 * An example of this process is shown below. 
 *
 * \htmlonly
 * <IMG SRC="spectral_bisection.png" width=700px height=400px align="middle"/>
 * \endhtmlonly
 *
 * Arnoldi iteration is used to compute the second smallest eigenvalue (through ARPACK). 
 * For larger datasets, this process is too expensive and so the matrix must first 
 * be coarsened before applying spectral bisection. See
 * coarsen.h for more information on the coarsening process.
 *
 */


#ifndef SPECTRALBISECTION_H
#define SPECTRALBISECTION_H

#include"graph.h"
#include<vector>

/**
 * \brief Performs spectral bisection on a graph  
 *
 * \param G Graph object
 * \return indices of reordered graph 
 */
std::vector<int> spectralBisection(Graph G);


/**
 * \brief Gets the index map for the reordered graph
 *
 * \param Eigvec2 Vector cooresponding to the second eigenvalue
 * \return indices of reordered graph based on eigenvalue
 */
std::vector<int> getIndexMap(std::vector<double> Eigvec2);
#endif
