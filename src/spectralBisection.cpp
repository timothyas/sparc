#include<iostream>
#include<iomanip>
#include<math.h>
#include"graph.h"
#include"spectralBisection.h"
#include<ctime>
#include<algorithm>
#include<fstream>
#include<vector>
#include<iterator>
#include "arlsmat.h"
#include "areig.h"
#include "arlssym.h"
using namespace std;


std::vector<int> spectralBisection(Graph *G)
{

	//Compute adjacency matrix and graph laplacian
	double start = std::clock();
	cout << "Computing adjacency matrix" << endl;
	CSC_MATRIX adj = G->computeAdjacencyMatrix();
	double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	cout << setprecision(15) << "--->Time: " << duration << endl;

	start = std::clock();
	cout << "Computing graph laplacian" << endl;
	CSC_MATRIX lap = G->computeGraphLaplacian(adj);
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	cout << setprecision(15) << "--->Time: " << duration << endl;

	start = std::clock();
	cout << "Getting second eigenvector with Arpack++" << endl; 
	ARluSymMatrix<double> L(lap.n,lap.nnz,&lap.vals[0],&lap.irow[0],&lap.pcol[0],'L');
	int nev = 2; 
	int ncv = min(2*nev+1,lap.n -1);
	ARluSymStdEig<double> prob(nev,L,"SA",ncv,0,1000000);
	prob.FindEigenvectors();

	double * Eigvec = prob.RawEigenvector(1);
	double * EigVal = prob.RawEigenvalues();
	cout << "2nd eigenvalue: " << EigVal[1] << endl;
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	cout << setprecision(15) << "--->Time: " << duration << endl;

	start = std::clock();
	vector<double> Eigvec2(Eigvec,Eigvec+lap.n);
	cout << "Getting index Map" << endl;
	std::vector<int> indMap =  getIndexMap(Eigvec2);
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	cout << setprecision(15) << "--->Time: " << duration << endl;

	return indMap; 
}

std::vector<int> getIndexMap(vector<double> Eigvec2)
{
	std::vector<int> ind1,ind2;
	std::vector<double> sortedEigvec2 = Eigvec2;  

	std::vector<int> indMap (Eigvec2.size(),0);

	std::sort(sortedEigvec2.data(),sortedEigvec2.data()+sortedEigvec2.size());
	double split = sortedEigvec2[sortedEigvec2.size()/2];

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

	ind1.insert(ind1.end(),ind2.begin(),ind2.end());
	
	for (size_t i = 0; i < ind1.size(); i++)
	{
		indMap[ind1[i]]=i;
	}

	return indMap;
}

int saveMatrixToFile(CSC_MATRIX A,std::string filename)
{
	ofstream outFile; 
	outFile.open(filename.c_str());

	int counter; 
	int numMatched;
	int row, col; 

	for (int i = 0; i < A.n;i++)
	{
		numMatched= 0;
		for (int j = 0; j < A.n; j++)
		{
			col = i;
			row = j;
			counter = A.pcol[col] + numMatched;
			if (counter < A.pcol[col+1] && A.irow[counter] == row)
			{
				outFile << A.vals[counter] << " ";
				numMatched++;
			}
			else 
				outFile << 0 << " ";
		}
		outFile << endl;
	}

	/*
	for (int i = 0; i < A.n; i++)
	{
		for (int j = 0; j < A.n; j++)
		{
			outFile << getAij(A,i,j) << " ";
		}
		outFile << endl;
	}
	*/
	outFile.close();
	return 0;
}
double getAij(CSC_MATRIX A,int i,int j)
{
	// flip indices if in upper triangular part of matrix
	if (j > i)
	{
		int temp = i; 
		i = j; 
		j = temp;
	}

	for (int n = A.pcol[j]; n < A.pcol[j+1]; n++)
	{
		if (A.irow[n] == i)
		{
			return A.vals[n];
		}
	}
	return 0.0; 
}
