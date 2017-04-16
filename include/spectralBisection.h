#ifndef SPECTRALBISECTION_H
#define SPECTRALBISECTION_H

#include<Eigen/Dense>
#include"graph.h"

using namespace Eigen;
/*   Calculate the adjacency matrix from the (coarsened)
 * list of edges.
 *
 * Inputs
 * bisectMe: graph object to compute adjacency matrix for 
 *
 * Outputs
 * adjMat: adjacency matrix associated with edge pairings 
 */
int computeAdjacencyMatrix(Graph* bisectMe, MatrixXd &adjMat);

/*   Calculate the out degree matrix associated 
 * with the (square) adjacency matrix.
 *
 * Inputs
 * adjMat: adjacency matrix
 * matLen: matrix length, adjMat is matLen x matLen
 *
 * Output
 * L: Graph laplacian
 */
int computeGraphLaplacian(MatrixXd &adjMat, MatrixXd &L, int matLen);


#endif
