#include<Eigen/Dense>
#include"graph.h"
#include"spectralBisection.h"

using namespace std;
using namespace Eigen;

int computeAdjacencyMatrix(Graph* bisectMe, MatrixXd &adjMat)
{ 

	int m,n;

	// Adjacency mat gets 1 where there's a connection btwn 2 nodes
	// 	may need to replace with weights ...
	for( int i=0; i<bisectMe->getNumEdges(); i++)
	{
		m = (int)bisectMe->getEdgePoint(i,0);
		n = (int)bisectMe->getEdgePoint(i,1);
		
		adjMat(m,n) = 1;
		adjMat(n,m) = 1;
	}

	return 0;
}

int computeGraphLaplacian(MatrixXd &adjMat, MatrixXd &L, int matLen) {


	// Sum along columns to get degrees
	VectorXd partialSum = adjMat.colwise().sum();
	
	// Diagonal elements L = D-A
	for( int i = 0; i<matLen; i++)
		L(i,i) = partialSum(i) - adjMat(i,i);

	// Off diagonals
	for( int i = 0; i<matLen; i++)
	{
	  for( int j = i+1; j<matLen; j++)
	  {
		L(i,j) = -adjMat(i,j);
		L(j,i) = -adjMat(j,i);
	  }
	}

	return 0;
}