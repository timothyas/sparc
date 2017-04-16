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
	MatrixXd A = ptr->computeAdjacencyMatrix();
	MatrixXd L = ptr->computeGraphLaplacian();

	// Output for check
	cout << "Adjacency matrix: " << endl;
	cout << A(0,0) << " " << A(0,1) << " " << A(0,2) << " "<< A(0,3) << endl;
	cout << A(1,0) << " " << A(1,1) << " " << A(1,2) << " " << A(1,3) << endl;
	cout << A(2,0) << " " << A(2,1) << " " << A(2,2) << " "<< A(2,3) << endl;
	cout << A(3,0) << " " << A(3,1) << " " << A(3,2) << " " << A(3,3) << endl;
	cout << "\n --- \n " << endl;

	cout << "Laplacian: " << endl;
	cout << L(0,0) << " " << L(0,1) << " " << L(0,2) << " " << L(0,3)<< endl;
	cout << L(1,0) << " " << L(1,1) << " " << L(1,2) << " " << L(1,3) << endl;
	cout << L(2,0) << " " << L(2,1) << " " << L(2,2) << " " << L(2,3)<< endl;
	cout << L(3,0) << " " << L(3,1) << " " << L(3,2) << " " << L(3,3) << endl;
	cout << "\n --- \n " << endl;
	// cout << "Graph laplacian: " << endl;
	// cout << L << " \n --- \n " << endl;

	return 0; 
}
