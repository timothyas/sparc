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


std::vector<int> spectralBisection(Graph G)
{

	//Compute adjacency matrix and graph laplacian
	double start = std::clock();
	cout << "--->Computing adjacency matrix ";
	CSC_MATRIX adj = G.computeAdjacencyMatrix();
	double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	cout << setprecision(15) << "(Time: " << duration << ")" <<endl;

	start = std::clock();
	cout << "--->Computing graph laplacian " ;
	CSC_MATRIX lap = G.computeGraphLaplacian(adj);
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	cout << setprecision(15) << "(Time: " << duration << ")" << endl;

	start = std::clock();
	cout << "--->Getting second eigenvector with Arpack++ ";
	ARluSymMatrix<double> L(lap.n,lap.nnz,&lap.vals[0],&lap.irow[0],&lap.pcol[0],'L');
	int nev = 2; 
	int ncv = min(2*nev+1,lap.n -1);
	ARluSymStdEig<double> prob(nev,L,"SM",ncv,0,1000000);
	prob.FindEigenvectors();

	double * Eigvec = prob.RawEigenvector(1);
	double * EigVal = prob.RawEigenvalues();
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	cout << setprecision(15) << "Time: " << duration << ")" << endl;

	start = std::clock();
	vector<double> Eigvec2(Eigvec,Eigvec+lap.n);
	cout << "--->Getting index Map ";
	std::vector<int> indMap =  getIndexMap(Eigvec2);
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	cout << setprecision(15) << "(Time: " << duration << ")" << endl;

	return indMap; 
}

std::vector<int> getIndexMap(vector<double> Eigvec2)
{
	std::vector<int> ind1,ind2;
	std::vector<double> sortedEigvec2(Eigvec2);  

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

	return ind1;
}

