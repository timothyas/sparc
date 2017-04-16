#include<Eigen/Eigenvalues>
#include<iostream>
#include<iomanip>
#include<math.h>
#include<Eigen/Dense>
#include"graph.h"
#include"spectralBisection.h"

using namespace std;
using namespace Eigen;

MatrixXd spectralBisection(Graph * G)
{
	//Compute adjacency matrix and graph laplacian
	MatrixXd A = G->computeAdjacencyMatrix();
	MatrixXd L = G->computeGraphLaplacian();

	//get second eigenvector
	VectorXd eigvec2;
	SelfAdjointEigenSolver<MatrixXd> eigensolver(L);
	VectorXd eigvec2 = eigensolver.eigenvectors().col(1);

	//Get new index sets
	VectorXd ind1,ind2; 
	getIndexSets(eigvec2,ind1,ind2);

	MatrixXd newA = MatrixXd::Zero(A.rows(),A.cols());
	MatrixXd newA11 = MatrixXd::Zero(ind1.size(),ind1.size());
	MatrixXd newA12 = MatrixXd::Zero(ind1.size(),ind2.size());
	MatrixXd newA21 = MatrixXd::Zero(ind2.size(),ind1.size());
	MatrixXd newA22 = MatrixXd::Zero(ind2.size(),ind2.size());

	getBlocks(A,newA11,newA12,newA21,newA22);

	newA << newA11, newA12, newA21,newA22; 
	return newA;
}

void getBlocks(MatrixXd A, VectorXd ind1, VectorXd in2, MatrixXd & newA11,MatrixXd & newA12,MatrixXd & newA21,MatrixXd & newA22)
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
