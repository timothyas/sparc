#include<iostream>
#include<iomanip>
#include<math.h>
#include"graph.h"
#include"spectralBisection.h"
#include<ctime>
#include<algorithm>
#include<fstream>
#include<vector>
#include "arlsmat.h"
#include "areig.h"
#include "arlssym.h"
using namespace std;


int spectralBisection(Graph *G)
{

	std::string filename = "facebook_SB.txt";
	//Compute adjacency matrix and graph laplacian
	cout << "Computing adjacency matrix" << endl;
	CSC_MATRIX adj = G->computeAdjacencyMatrix();
	cout << "Computing graph laplacian" << endl;
	CSC_MATRIX lap = G->computeGraphLaplacian(adj);

	cout << "Getting second eigenvector with Arpack++" << endl; 

	
	double start = std::clock();
	ARluSymMatrix<double> L(lap.n,lap.nnz,&lap.vals[0],&lap.irow[0],&lap.pcol[0],'L');
	ARluSymStdEig<double> prob(2,L,"SA",5,0,100000);
	prob.FindEigenvectors();
	double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	cout << "Time: " << duration << endl;

	double * Eigvec = prob.RawEigenvector(1);
	double * EigVal = prob.RawEigenvalues();
	cout << "2nd eigenvalue" << EigVal[1] << endl;

	vector<double> Eigvec2(Eigvec,Eigvec+lap.n);
	std::vector<int> ind1,ind2;

	cout << "Getting index sets" << endl;
	getIndexSets(Eigvec2, ind1, ind2);
	cout << "Performing swapping of indices" << endl;
	CSC_MATRIX A_SB = getBlocks(adj, ind1,ind2);
	cout << "Saving matrix" << endl;
	saveMatrixToFile(A_SB,filename);
	return 0; 
}

CSC_MATRIX getBlocks(CSC_MATRIX A, std::vector<int> ind1, std::vector<int> ind2)
{
	CSC_MATRIX A_SB;
	A_SB.n = A.n; 
	A_SB.nnz = A.nnz;
	A_SB.irow.reserve(A.irow.size());
	A_SB.pcol.reserve(A.pcol.size());
	A_SB.vals.reserve(A.vals.size());
	double value; 
	double pcolSet; // determine if pcol has already been set for each row

	for (size_t j = 0; j < A_SB.n; j++)
	{
		pcolSet = false;
		for (size_t i = j; i < A_SB.n; i++)
		{
			cout << "i: " << i << " j: " << j << endl;
		
			// in A11 block
			if (j < ind1.size() && i < ind1.size())
			{
				// check element A(ind1(i),ind1(j)) is non zero
				double value = getAij(A,ind1[i],ind1[j]);
				if (value !=0)
				{
					A_SB.vals.push_back(value); 
					A_SB.irow.push_back(i);
					if (!pcolSet)
					{
						A_SB.pcol.push_back(A_SB.vals.size()-1);
						pcolSet = true; 
					}

				}
			}

			// in A21
			else if (j < ind1.size() && i >= ind1.size())
			{
				// check element A(ind1(i),ind1(j)) is non zero
				double value = getAij(A,ind2[i-ind1.size()],ind1[j]);
				if (value !=0)
				{
					A_SB.vals.push_back(value); 
					A_SB.irow.push_back(i);
					if (!pcolSet)
					{
						A_SB.pcol.push_back(A_SB.vals.size()-1);
						pcolSet = true; 
					}

				}
			}


			// in A12
			else if (j >= ind1.size() && i < ind1.size())
			{
				// check element A(ind1(i),ind1(j)) is non zero
				double value = getAij(A,ind1[i],ind2[j-ind1.size()]);
				if (value !=0)
				{
					A_SB.vals.push_back(value); 
					A_SB.irow.push_back(i);
					if (!pcolSet)
					{
						A_SB.pcol.push_back(A_SB.vals.size()-1);
						pcolSet = true; 
					}

				}
			}

			// in A22 
			else if (j >= ind1.size() && i >= ind1.size())
			{
				// check element A(ind1(i),ind1(j)) is non zero
				double value = getAij(A,ind2[i-ind1.size()],ind2[j-ind1.size()]);
				if (value !=0)
				{
					A_SB.vals.push_back(value); 
					A_SB.irow.push_back(i);
					if (!pcolSet)
					{
						A_SB.pcol.push_back(A_SB.vals.size()-1);
						pcolSet = true; 
					}

				}
			}
		}
		if (!pcolSet)
		{
			A_SB.pcol.push_back(A_SB.pcol[A_SB.pcol.size()-1]);
		}
	}
	
	return A_SB;
}

int getIndexSets(vector<double> Eigvec2, std::vector<int> &ind1, std::vector<int> &ind2)
{
	vector<double> sortedEigvec2 = Eigvec2;  
	
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

	return 0;
}

int saveMatrixToFile(CSC_MATRIX A,std::string filename)
{
	ofstream outFile; 

	outFile.open(filename.c_str());

	for (int i = 0; i < A.n; i++)
	{
		for (int j = 0; j < A.n; j++)
		{
			outFile << getAij(A,i,j) << " ";
		}
		outFile << endl;
	}
	outFile.close();
	return 0;
}
double getAij(CSC_MATRIX A,int i,int j)
{
	int k = -1;  
	double ans;

	// flip indices if in upper triangular part of matrix
	if (j > i)
	{
		int temp = i; 
		i = j; 
		j = temp;
	}

	for (int n = A.pcol[j]; n<A.pcol[j+1]; n++)
	{
		if (A.irow[n] == i)
		{
			k = n-A.pcol[j];
			break;
		}
	}

	if (k == -1)
	{
		ans = 0;
	}
	else 
	{
		ans = A.vals[A.pcol[j]+k];
	}
	return ans; // not found
}
