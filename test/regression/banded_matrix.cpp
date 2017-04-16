#include<Eigen/Dense>
#include<iostream>
#include"graph.h"
#include"spectralBisection.h"

using namespace std;
using namespace Eigen;


int main()
{
	Graph banded_matrix("banded_matrix.dat");
	
	Graph* ptr = &banded_matrix;

	// Compute adjacency A and graph laplacian L
	int N = ptr->getNumNodes();
	MatrixXd A(N,N);
	MatrixXd L(N,N);

	computeAdjacencyMatrix(ptr,A);
	computeGraphLaplacian(A,L,N);

	// Output for check
	cout << "Adjacency matrix: " << endl;
	cout << A << "\n --- \n " << endl;

	cout << "Graph laplacian: " << endl;
	cout << L << " \n --- \n " << endl;

	return 0; 
}
