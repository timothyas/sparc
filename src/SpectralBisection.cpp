#include<Eigen/Dense>

using namespace std;
using namespace Eigen;

MatrixXd ComputeAdjacencyMatrix(MatrixX2d &edgeMat, int numNodes, int numEdges){ 

	MatrixXd adjMat(numNodes,numNodes); 

	// Adjacency mat gets 1 where there's a connection btwn 2 nodes
	// 	may need to replace with weights ...
	for( int i=0, i<numEdges, i++){
		adjMat(edgeMat(i,1),edgeMat(i,2)) = 1;
		adjMat(edgeMat(i,2),edgeMat(i,1)) = 1;
	}

	return adjMat;

}

MatrixXd ComputeGraphLaplacian(MatrixXd &adjMat, int matLen) {


	MatrixXd L(matLen,matLen);
	
	// Sum along columns to get degrees
	VectorXd partialSum = adjMat.colwise().sum();
	
	// Diagonal elements L = D-A
	for( int i = 0, i<matLen, i++)
		L(i,i) = partialSum(i) - adjMat(i,i);

	// Off diagonals
	for( i = 0, i<matLen, i++){
	  for( int j = i+1, i<matLen,j++){
		L(i,j) = -adjMat(i,j);
		L(j,i) = -adjMat(j,i);
	  }
	}

	return L;
}
