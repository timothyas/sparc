#include<Eigen/Dense>

using namespace std;
using namespace Eigen;

int ComputeAdjacencyMatrix(Graph* bisectMe, MatrixXd &adjMat){ 

	// Adjacency mat gets 1 where there's a connection btwn 2 nodes
	// 	may need to replace with weights ...
	for( int i=0, i<bisectMe->getNumEdges(), i++){
		adjMat(bisectMe->getEdgePoint(i,1),bisectMe->getEdgePoint(i,2)) = 1;
		adjMat(bisectMe->getEdgePoint(i,2),bisectMe->getEdgePoint(i,1)) = 1;
	}

	return 0;
}

int ComputeGraphLaplacian(MatrixXd &adjMat, MatrixXd &L, int matLen) {


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

	return 0;
}
