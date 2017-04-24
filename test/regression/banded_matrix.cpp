#include<iostream>
#include<iomanip>
#include"graph.h"
#include<math.h>
#include<ctime>
#include<vector>

using namespace std;

int banded_matrix()
{
	cout << "----------------------------------------------------------------------" << endl; 
	cout << "Running Banded Matrix Test" << endl; 
	cout << "----------------------------------------------------------------------" << endl; 

	Graph banded_matrix("banded_matrix.dat");

	CSC_MATRIX adj = banded_matrix.computeAdjacencyMatrix();
	return 0;
}

	/*
	// Compute adjacency A and graph laplacian L
	MatrixXd A = ptr->computeAdjacencyMatrix();
	MatrixXd L = ptr->computeGraphLaplacian();

	for (int i = 0; i < A.rows(); i++)
	{
		if (i==0)
		{
			if (A(i,i+1) !=1)
			{
				cout << "Computed Adjacency Matrix Incorrectly...exiting" << endl;
				return 1;
			}
			if (L(i,i) != 1 || L(i,i+1) !=-1)
			{
				cout << "Computed Graph Laplacian Incorrectly...exiting" << endl;
				return 1; 
			}
		}
		else if (i == A.rows()-1)
		{
			if (A(i,i-1) !=1)
			{
				cout << "Computed Adjacency Matrix Incorrectly...exiting" << endl;
				return 1;
			}
			if (L(i,i) != 1 || L(i,i-1) !=-1)
			{
				cout << "Computed Graph Laplacian Incorrectly...exiting" << endl;
				return 1; 
			}
		}

		else 
		{
			if (A(i,i+1) != 1 || A(i,i-1) !=1)
			{
			cout << "Computed Adjacency Matrix Incorrectly...exiting" << endl;
			return 1;
			}
			if (L(i,i+1) != -1 || L(i,i-1) != -1 || L(i,i) != 2)
			{
			cout << "Computed Graph Laplacian Incorrectly...exiting" << endl;
			return 1; 
			}
		}

	}

	cout << "Successfully Computed Graph Laplacian and Adjacency Matrix" << endl;

	std::clock_t start;
	double duration;

	start=std::clock();
	SelfAdjointEigenSolver<MatrixXd> eigensolver(L);
	double eigval2 = eigensolver.eigenvalues()[1];
	VectorXd eigvec2 = eigensolver.eigenvectors().col(1);
	duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Eigen duration: " << duration << endl; 
	VectorXd test1,test2; 
	test1 = L*eigvec2; 
	test2 = eigval2*eigvec2; 
	for (int i = 0; i < test1.size(); i++)
	{
		if (fabs(test1(i)-test2(i))>1e-7 )
		{
			cout << "Error finding second eigenvalue and/or vector...exiting" << endl; 
			return 1; 
		}
	}
	cout << "Successfully found second smallest eigenvalue and corresponding eigenvector" << endl;

	start = std::clock();
	DenseSymMatProd<double> op(L);
	SymEigsSolver< double, SMALLEST_ALGE, DenseSymMatProd<double> > eigs(&op, 2, 4);
	//SymEigsSolver< double, SMALLEST_ALGE, DenseSymMatProd<double> >::eigenvectors(2) //(&op, 2, 4);

	eigs.init();
	int nconv = eigs.compute(10000, 1e-10,LARGEST_ALGE);
	MatrixXd eigvec ;
	VectorXd eigvec2_Spectra;

	eigvec = eigs.eigenvectors();
	eigvec2_Spectra = eigvec.col(0);
	duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Eigen duration: " << duration << endl; 
	for (int i = 0; i < eigvec2.size(); i++)
	{
		if (fabs(eigvec2(i)-eigvec2_Spectra(i))>1e-7 )
		{
			cout << eigvec2(i) << " " << eigvec2_Spectra(i) << endl;
			cout << "Error finding second eigenvalue and/or vector with Spectra...exiting" << endl; 
			return 1; 
		}
	}
	cout << "Successfully found second smallest eigenvalue and corresponding eigenvector with Spectra" << endl;
	
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
	if (countLeft != countRight)
	{
		cout << "Error finding median...exiting" << endl;
		return 1;
	}
	cout << "Successfully sorted eigenvector and found median" << endl; 
	VectorXd ind1 = VectorXd::Zero(countLeft);
	VectorXd ind2 = VectorXd::Zero(countRight+1); 

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

	MatrixXd newA = MatrixXd::Zero(A.rows(),A.cols());
	MatrixXd newA11 = MatrixXd::Zero(ind1.size(),ind1.size());
	MatrixXd newA12 = MatrixXd::Zero(ind1.size(),ind2.size());
	MatrixXd newA21 = MatrixXd::Zero(ind2.size(),ind1.size());
	MatrixXd newA22 = MatrixXd::Zero(ind2.size(),ind2.size());

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

	newA << newA11, newA12, newA21,newA22; 

	return 0; 
}
*/
