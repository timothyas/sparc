#include<iostream>
#include<iomanip>
#include<math.h>
#include<ctime>
#include<vector>
#include"graph.h"
#include "arlsmat.h"
#include "areig.h"
#include "arlssym.h"
using namespace std;

int corner_cases_graph()
{
	cout << "----------------------------------------------------------------------" << endl; 
	cout << "Running Complex Graph Test" << endl; 
	cout << "----------------------------------------------------------------------" << endl; 

	Graph complex_graph("corner_cases_graph.dat");

	CSC_MATRIX adj = complex_graph.computeAdjacencyMatrix();

	int irow_temp[] = {1,3};
	int pcol_temp[] = {0,1,1,2,2};
	int vals_temp[] = {1,1,1,1,1};
	std::vector<int> irow_test (irow_temp, irow_temp + sizeof(irow_temp) / sizeof(int) );	
	std::vector<int> pcol_test (pcol_temp, pcol_temp + sizeof(pcol_temp) / sizeof(int) );	
	std::vector<int> vals_test (vals_temp, vals_temp + sizeof(vals_temp) / sizeof(int) );	

	for (size_t i = 0; i < adj.irow.size(); i++)
	{
		if (adj.irow[i] != irow_test[i])
		{
			cout << "Expected: " << irow_test[i] << "\tGot: " << adj.irow[i] << endl;
			cout << "Error computing adjacency matrix [irow]...exiting" << endl;
			return 1; 
		}
	}

	for (size_t i = 0; i < adj.vals.size(); i++)
	{
		if (adj.vals[i] != vals_test[i])
		{
			cout << "Expected: " << vals_test[i] << "\tGot: " << adj.vals[i] << endl;
			cout << "Error computing adjacency matrix [vals]...exiting" << endl;
			return 1; 
		}
	}
	
	for (size_t i = 0; i < adj.pcol.size(); i++)
	{
		if (adj.pcol[i] != pcol_test[i])
		{
			cout << "Expected: " << pcol_test[i] << "\tGot: " << adj.pcol[i] << endl;
			cout << "Error computing adjacency matrix [pcol]...exiting" << endl;
			return 1; 
		}
	}
		
	cout << "Successfully computed adjacency matrix in CSC format" << endl;

	CSC_MATRIX lap = complex_graph.computeGraphLaplacian(adj);
	int irowtemp[] = {0,1,1,2,3,3};
	int pcoltemp[] = {0,2,3,5,6};
	int valstemp[] = {1,-1,1,1,-1,1};
	std::vector<int> test_irow (irowtemp, irowtemp + sizeof(irowtemp) / sizeof(int) );	
	std::vector<int> test_pcol (pcoltemp, pcoltemp + sizeof(pcoltemp) / sizeof(int) );	
	std::vector<int> test_vals (valstemp, valstemp + sizeof(valstemp) / sizeof(int) );	

	
	for (size_t i = 0; i < lap.pcol.size(); i++)
	{
		if (lap.pcol[i] != test_pcol[i])
		{
			cout << "Expected: " << test_pcol[i] << "\tGot: " << lap.pcol[i] << endl;
			cout << "Error computing graph laplacian [pcol]...exiting" << endl;
			return 1; 
		}
	}

	for (size_t i = 0; i < lap.irow.size(); i++)
	{
		if (lap.irow[i] != test_irow[i])
		{
			cout << "Expected: " << test_irow[i] << "\tGot: " << lap.irow[i] << endl;
			cout << "Error computing graph laplacian [irow]...exiting" << endl;
			return 1; 
		}
	}

	for (size_t i = 0; i < lap.vals.size(); i++)
	{
		if (lap.vals[i] != test_vals[i])
		{
			cout << "Expected: " << test_vals[i] << "\tGot: " << lap.vals[i] << endl;
			cout << "Error computing graph laplacian [vals]...exiting" << endl;
			return 1; 
		}
	}
		
	cout << "Successfully computed graph laplacian in CSC format" << endl;

	return 0;
}
