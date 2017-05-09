#include<iostream>
#include<iomanip>
#include<math.h>
#include<ctime>
#include<vector>
#include<spectralBisection.h>
#include"graph.h"
#include "arlsmat.h"
#include "areig.h"
#include "arlssym.h"
using namespace std;

int simple_graph()
{
	cout << "----------------------------------------------------------------------" << endl; 
	cout << "Running Simple Graph Test" << endl; 
	cout << "----------------------------------------------------------------------" << endl; 

	Graph simple_mat("simple_graph.dat");
//	Graph * ptr = &simple_mat;

	std::vector<std::vector<int> > NE (simple_mat.getNumNodes());
	NE[0].push_back(1); NE[0].push_back(3);
	NE[1].push_back(0); NE[1].push_back(2); NE[1].push_back(3);
	NE[2].push_back(1); NE[2].push_back(3);
	NE[3].push_back(0); NE[3].push_back(1); NE[3].push_back(2);

	for (int i = 0; i < simple_mat.getNumNodes(); i++)
	{
		std::vector<int> NE_COMP = simple_mat.getNeighbors(i);
		std::vector<int> NE_TEST = NE[i];
		for (size_t j = 0; j < NE_COMP.size(); j++)
			if (NE_COMP[j] != NE_TEST[j])
			{
				cout << "Error computing neighbors...exiting" << endl;
				return 1;
			}
	}
	
	cout << "Successfully computed neighbors" << endl;

	CSC_MATRIX adj = simple_mat.computeAdjacencyMatrix();

	int irow_temp[] = {1,3,2,3,3};
	int pcol_temp[] = {0,2,4,5,5};
	int vals_temp[] = {1,1,1,1,1};
	std::vector<int> irow_test (irow_temp, irow_temp + sizeof(irow_temp) / sizeof(int) );	
	std::vector<int> pcol_test (pcol_temp, pcol_temp + sizeof(pcol_temp) / sizeof(int) );	
	std::vector<int> vals_test (vals_temp, vals_temp + sizeof(vals_temp) / sizeof(int) );	

	for (size_t i = 0; i < adj.irow.size(); i++)
		if (adj.irow[i] != irow_test[i])
		{
			cout << "Expected: " << irow_test[i] << "\tGot: " << adj.irow[i] << endl;
			cout << "Error computing adjacency matrix [irow]...exiting" << endl;
			return 1; 
		}

	for (size_t i = 0; i < adj.vals.size(); i++)
		if (adj.vals[i] != vals_test[i])
		{
			cout << "Expected: " << vals_test[i] << "\tGot: " << adj.vals[i] << endl;
			cout << "Error computing adjacency matrix [vals]...exiting" << endl;
			return 1; 
		}
	
	for (size_t i = 0; i < adj.pcol.size(); i++)
		if (adj.pcol[i] != pcol_test[i])
		{
			cout << "Expected: " << pcol_test[i] << "\tGot: " << adj.pcol[i] << endl;
			cout << "Error computing adjacency matrix [pcol]...exiting" << endl;
			return 1; 
		}
		
	cout << "Successfully computed adjacency matrix in CSC format" << endl;


	CSC_MATRIX lap = simple_mat.computeGraphLaplacian(adj);
	int irowtemp[] = {0,1,3,1,2,3,2,3,3};
	int pcoltemp[] = {0,3,6,8,9};
	int valstemp[] = {2,-1,-1,3,-1,-1,2,-1,3};
	std::vector<int> test_irow (irowtemp, irowtemp + sizeof(irowtemp) / sizeof(int) );	
	std::vector<int> test_pcol (pcoltemp, pcoltemp + sizeof(pcoltemp) / sizeof(int) );	
	std::vector<int> test_vals (valstemp, valstemp + sizeof(valstemp) / sizeof(int) );	

	for (size_t i = 0; i < lap.irow.size(); i++)
		if (lap.irow[i] != test_irow[i])
		{
			cout << "Expected: " << test_irow[i] << "\tGot: " << lap.irow[i] << endl;
			cout << "Error computing graph laplacian [irow]...exiting" << endl;
			return 1; 
		}

	for (size_t i = 0; i < lap.vals.size(); i++)
		if (lap.vals[i] != test_vals[i])
		{
			cout << "Expected: " << test_vals[i] << "\tGot: " << lap.vals[i] << endl;
			cout << "Error computing graph laplacian [vals]...exiting" << endl;
			return 1; 
		}
	
	for (size_t i = 0; i < lap.pcol.size(); i++)
		if (lap.pcol[i] != test_pcol[i])
		{
			cout << "Expected: " << test_pcol[i] << "\tGot: " << lap.pcol[i] << endl;
			cout << "Error computing graph laplacian [pcol]...exiting" << endl;
			return 1; 
		}
		
	cout << "Successfully computed graph laplacian in CSC format" << endl;

	
	
	ARluSymMatrix<double> L(lap.n,lap.nnz,&lap.vals[0],&lap.irow[0],&lap.pcol[0],'L');
	int nev = 2; 
	int ncv = min(2*nev+1,lap.n);

	ARluSymStdEig<double> prob(nev,L,"SM",ncv,0,1000000000);
	prob.FindEigenvectors();
	double * Eigvec = prob.RawEigenvector(0);
	double * EigVal = prob.RawEigenvalues();

	for (int i = 0; i < simple_mat.getNumNodes();i++)
	{
		cout << EigVal[0] << endl;
		cout << *(Eigvec+i) << endl;
	}

	if (fabs(EigVal[1]- 2) > 1e-10)
	{
		cout << "Error computing eigenvalues...exiting" << endl;
		return 1; 
	}
	cout << "Successfully computed smallest two eigenvalues" << endl;
	
	return 0;
}
