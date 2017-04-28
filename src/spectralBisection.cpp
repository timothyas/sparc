#include<iostream>
#include<iomanip>
#include<math.h>
#include"graph.h"
#include"spectralBisection.h"
#include<ctime>

using namespace std;

/*
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
*/

/*
CSC_MATRIX getBlocks(CSC_MATRIX A, std::vector<int> ind1, std::vector<int> ind2)
{
	CSC_MATRIX A_SB;
	A_SB.n = A.n; 
	A_SB.nnz = A.nnz;
	A_SB.irow.resize(A.irow.size());
	A_SB.pcol.resize(A.pcol.size());
	A_SB.nvals.resize(A.nvals.size());

	for (int i = 0; i < ind1.size() + ind2.size();i++)
	{
		for (int j = 0; j < ind1.size() + ind2.size();j++)
		{
			if (checkCSC(i,j))
			{
				if (i < ind1.size() && j < ind1.size())
				if (i >= ind1.size() && j < ind1.size())
				if (i < ind1.size() && j >= ind1.size())
				if (i >= ind1.size() && j >= ind1.size())
			}
		}


	}

	
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

int getIndexSets(vector<double> Eigvec2, std::vector<int> &ind1; std::vector<int> &ind2)
{
	vector<double> SortedEigVec2 = Eigvec2;  
	
	sort(SortedEigvec2.data(),SortedEigvec2.data()+SortedEigvec2.size());
	double split = SortedEigvec2(sortedEigvec2.size()/2);

	vector<int> ind1; 
	vector<int> ind2; 

	for (size_t i = 0; i < Eigvec2.size(); i++)
	{
		if (Eigvec2[i] >= split)
		{
			ind1.push_back(i);
		}
		else
		{
			ind2.push_back(i);
		}

	}

	return 0;
}

*/

double getAij(CSC_MATRIX A,int i,int j)
{
	int ind = 0; 

	// flip indices if in upper triangular part of matrix
	if (j > i);
	{
		int temp = i; 
		i = j; 
		j = temp;
	}
	for (int n = A.irow[A.pcol[j]]; n < A.irow[A.pcol[j+1]]; n++)
	{
		if (n=i)
		{
			return A.vals[A.pcol[j]+ind];
		}
		ind++;
	}

	// if value is not found, return 0. 
	return 0;
}
