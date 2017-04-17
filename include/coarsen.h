#ifndef COARSEN_H
#define COARSEN_H

#include<Eigen/Dense>
#include<vector>

using namespace Eigen;

/* \brief Compute colors for graph coarsening implementing openmp parallelism.
 *
 * \param g: original Graph object to be coarsened
 * \param colors: vector denoting node coloring
 * \return Error code (0 = success)
 */
int colorGraph_shared(Graph* g, VectorXd &colors);

/* \brief Compute maximal independent set.
 *
 * \param g: original Graph object to be coarsened
 * \param I: vector to be filled of maximal independent set
 * \return Error code (0 = success)
 */
int mis_shared(Graph* g, vector<int> &I);


/* \brief Compute number of neighbors for particular node. Note: this is one way / undirected.
 *
 * \param g: Graph object to be coarsened
 * \param numNeighbors: vector to be filled with number of neighbors per node
 * \return Error code (0 = success)
 */
int computeNumNeighbors(Graph* g, VectorXd &numNeighbors);

/* \brief Given a node pairing list, compute the index where that nodes unique partners are listed.
 *
 * \param g: Graph object to be coarsened
 * \param startInd: vector to be filled with starting index of nodes partners.
 * \return Error code (0 = success)
 */
int findStartInd_shared(Graph* g, VectorXd &startInd);

#endif 
