#include<Eigen/Eigenvalues>
#include"SymEigsSolver.h"
#include<iostream>
#include<iomanip>
#include<math.h>
#include<Eigen/Dense>
#include"graph.h"
#include"spectralBisection.h"
#include<ctime>

using namespace std;
using namespace Eigen;

MatrixXd spectralBisection(Graph * G)
{



	//Compute adjacency matrix and graph laplacian
	cout << "Computing adjacency matrix" << endl;
	MatrixXd A = G->computeAdjacencyMatrix();
	cout << "Computing graph laplacian" << endl;
	MatrixXd L = G->computeGraphLaplacian();

	//get second eigenvector
	cout << "Getting second eigenvector with Eigen" << endl; 
	std::clock_t start;
	double duration;
	start = std::clock();
	SelfAdjointEigenSolver<MatrixXd> eigensolver(L);
	double eigval2 = eigensolver.eigenvalues()[1];
	VectorXd eigvec2 = eigensolver.eigenvectors().col(1);
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	cout << "Eigen duration" << duration << endl;
	//
	
	start = std::clock();
	cout << "Getting second eigenvector with Spectra" << endl; 
	//
	DenseSymMatProd<double> op(L);
	SymEigsSolver< double, SMALLEST_ALGE, DenseSymMatProd<double> > eigs(&op, 2, 4);
	eigs.init();
	int nconv = eigs.compute(10000, 1e-3,LARGEST_ALGE);
	MatrixXd eigvec ;
	VectorXd eigvec2_spectra;
	cout << "---> Number of converged eigenvlues: " << nconv << endl;
	eigvec = eigs.eigenvectors();
	eigvec2_spectra = eigvec.col(0);
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	cout << "Spectra duration" << duration << endl;


	//Get new index sets
	cout << "Creating index sets" << endl;
	VectorXd ind1,ind2; 
	getIndexSets(eigvec2,ind1,ind2);

	MatrixXd newA = MatrixXd::Zero(A.rows(),A.cols());
	MatrixXd newA11 = MatrixXd::Zero(ind1.size(),ind1.size());
	MatrixXd newA12 = MatrixXd::Zero(ind1.size(),ind2.size());
	MatrixXd newA21 = MatrixXd::Zero(ind2.size(),ind1.size());
	MatrixXd newA22 = MatrixXd::Zero(ind2.size(),ind2.size());

	cout << "Getting block of spectral bisection" << endl;
	getBlocks(A,ind1,ind2,newA11,newA12,newA21,newA22);

	newA << newA11, newA12, newA21,newA22; 
	return newA;
}

void getBlocks(MatrixXd A, VectorXd ind1, VectorXd ind2, MatrixXd & newA11,MatrixXd & newA12,MatrixXd & newA21,MatrixXd & newA22)
{
	for (int i = 0; i < ind1.size(); i++)
	{
		for (int j = 0; j < ind1.size(); j++)
		{
			newA11(i,j) = A(ind1(i),ind1(j)); 
		}
	}

	for (int i = 0; i < ind1.size(); i++)
	{
		for (int j = 0; j < ind2.size(); j++)
		{
			newA12(i,j) = A(ind1(i),ind2(j)); 
		}
	}
	for (int i = 0; i < ind2.size(); i++)
	{
		for (int j = 0; j < ind1.size(); j++)
		{
			newA21(i,j) = A(ind2(i),ind1(j)); 
		}
	}

	for (int i = 0; i < ind2.size(); i++)
	{
		for (int j = 0; j < ind2.size(); j++)
		{
			newA22(i,j) = A(ind2(i),ind2(j));
		}
	}
}

int getIndexSets(double * Eigvec2, std::vector<int> &ind1; std::vector<int> &ind2)
{

	return 0;

}

/*	
void getIndexSets(VectorXd eigvec2,VectorXd & ind1,VectorXd & ind2)
{
	VectorXd sortedeigvec2 = eigvec2;
	sort(sortedeigvec2.data(),sortedeigvec2.data()+sortedeigvec2.size());
	double split = sortedeigvec2(sortedeigvec2.size()/2);
	int countLeft =0;
	int countRight = 0;

	for (int i = 0; i < eigvec2.size(); i++)
	{
		if (eigvec2(i) < split)
			countLeft++; 
		if (eigvec2(i) > split) 
			countRight++; 
	}
	ind1 = VectorXd::Zero(countLeft);
	ind2 = VectorXd::Zero(countRight+1); 

	countLeft = 0; 
	countRight = 0; 
	for (int i = 0; i < eigvec2.size(); i++)
	{
		if (eigvec2(i) < split)
		{
			ind1(countLeft) = i; 
			countLeft++; 
		}
		if (eigvec2(i) >= split)
		{
			ind2(countRight) = i; 
			countRight++;
		}
	}

}
*/
