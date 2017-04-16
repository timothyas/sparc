#include<Eigen/Dense>


/*   Calculate the adjacency matrix from the (coarsened)
 * list of edges.
 *
 * Inputs
 * bisectMe: graph object to compute adjacency matrix for 
 *
 * Outputs
 * adjMat: adjacency matrix associated with edge pairings 
 */
MatrixXd ComputeAdjacencyMatrix(Graph* bisectMe, MatrixXd &adjMat)

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
MatrixXd ComputeGraphLaplacian(MatrixXd &adjMat, int matLen)





