#ifndef COARSEN_H
#define COARSEN_H

#include<vector>

/* \brief Compute colors for graph coarsening implementing openmp parallelism.
 *
 * \param g: original Graph object to be coarsened
 * \param colors: vector denoting node coloring
 * \return Error code (0 = success)
 */
int colorGraph_shared(Graph* g, vector<int> &colors);

/* \brief Compute maximal independent set.
 *
 * \param g: original Graph object to be coarsened
 * \param finalRemoveList: vector of nodes which have already been assigned a color
 * \param I: vector to be filled of maximal independent set
 * \return Error code (0 = success)
 */
int mis_shared(Graph* g, vector<bool> &finalRemoveList, vector<int> &I);


/* \brief Find neighbors of a particular node and return list of node numbers.
 *
 * \param g: Graph object to be coarsened
 * \param removedNodes: vector of nodes which have been removed from current search
 * \param u: node which we want neighbors of 
 * \param neighbors: vector of node numbers neighboring u
 * \return Error code (0 = success)
 */
int findNeighbors_shared(Graph* g, vector<int> &removedNodes, int u, vector<int> &neighbors);

/* \brief Performs parallel inclusive scan on vector. Does not use pass by reference since algorithm is recursive..
 *
 * \param a: vector of integers to be scanned
 * \return s: cumulative sum of vector a 
 */
int inclusiveScan_shared(vector<int> a);

#endif 
