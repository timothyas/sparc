#include"graph.h"

int main()
{
	Graph* banded_matrix("banded_matrix.dat");


	// Compute adjacency A and graph laplacian L
	int N = banded_matrix->getNumNodes();
	MatrixXd A(N,N);
	MatrixXd L(N,N);

	ComputeAdjacencyMatrix(banded_matrix,A);
	ComputeGraphLaplacian(A,L,N);

	// Output for check
	cout << "Adjacency matrix: " << endl;
	cout << A << "\n --- \n " << end;

	cout << "Graph laplacian: " << endl;
	cout << L << " \n --- \n " << endl;

	return 0; 
}
