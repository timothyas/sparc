#include<Eigen/Dense>


/*   Calculate the adjacency matrix from the (coarsened)
 * list of edges.
 *
 * Inputs
 * edgeMat: M x 2 matrix containing list of edges
 * numNodes: number of nodes in graph
 * edges: number of edges in graph, number of rows in edgeMat
 */
MatrixXd ComputeAdjacencyMatrix(MatrixX2d &edgeMat, int numNodes, int numEdges)

/*   Calculate the out degree matrix associated 
 * with the (square) adjacency matrix.
 *
 * Inputs
 * adjMat: adjacency matrix
 * matLen: matrix length, adjMat is matLen x matLen
 */
MatrixXd ComputeGraphLaplacian(MatrixXd &adjMat, int matLen)





