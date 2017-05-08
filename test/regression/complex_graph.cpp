#include<iostream>
#include<iomanip>
#include<math.h>
#include<ctime>
#include<vector>
#include"spectralBisection.h"
#include"graph.h"
#include"coarsen.h"
#include "arlsmat.h"
#include "areig.h"
#include "arlssym.h"
using namespace std;

int complex_graph()
{
	cout << "----------------------------------------------------------------------" << endl; 
	cout << "Running Complex Graph Test" << endl; 
	cout << "----------------------------------------------------------------------" << endl; 

	Graph complex_graph("complex_graph.dat");

	CSC_MATRIX adj = complex_graph.computeAdjacencyMatrix();

        vector<double> timeKeeper(4,0.0);
	int irow_temp[] = {1,6,9,2,3,4,6,5,6,7,8};
	int pcol_temp[] = {0,3,5,5,7,8,9,10,11,11,11};
	int vals_temp[] = {1,1,1,1,1,1,1,1,1,1,1};
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
	int irowtemp[] = {0,1,6,9,1,2,3,2,3,4,6,4,5,5,6,6,7,7,8,8,9};
	int pcoltemp[] = {0,4,7,8,11,13,15,17,19,20,21};
	int valstemp[] = {3,-1,-1,-1,3,-1,-1,1,3,-1,-1,2,-1,2,-1,4,-1,2,-1,1,1};
	std::vector<int> test_irow (irowtemp, irowtemp + sizeof(irowtemp) / sizeof(int) );	
	std::vector<int> test_pcol (pcoltemp, pcoltemp + sizeof(pcoltemp) / sizeof(int) );	
	std::vector<int> test_vals (valstemp, valstemp + sizeof(valstemp) / sizeof(int) );	

	for (size_t i = 0; i < lap.irow.size(); i++)
	{
		if (lap.irow[i] != test_irow[i])
		{
			cout << "Expected: " << test_irow[i] << "\tGot: " << lap.irow[i] << endl;
			cout << "Error computing graph laplacian [irow]...exiting" << endl;
			return 1; 
		}
	}
	
	for (size_t i = 0; i < lap.pcol.size(); i++)
	{
		if (lap.pcol[i] != test_pcol[i])
		{
			cout << "Expected: " << test_pcol[i] << "\tGot: " << lap.pcol[i] << endl;
			cout << "Error computing graph laplacian [pcol]...exiting" << endl;
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

	Graph Graph1; 
	Graph Graph2;

	Graph1.coarsenFrom(complex_graph,timeKeeper);
	Graph2.coarsenFrom(Graph1,timeKeeper);
	std::vector<int> indMap = spectralBisection(Graph2);

	/*
	cout << "SB Index Map" << endl; 
	for (int i = 0; i < indMap.size(); i++)
	{
		cout << indMap[i] << endl;
	}

        cout << " --- Child Match list Graph 2 --- " << endl;
        for( int i=0; i<Graph2.getNumNodes(); i++){
          cout << Graph2.getNodeMatch(i) << endl;
        }

	cout << "Graph 2 Before" << endl;
	for (int i = 0; i < Graph2.getNumNodes(); i++)
	{
		std::vector<int> C = Graph2.getChildren(i);
		for (int j = 0; j < C.size(); j++)
		{
			cout << C[j] << " ";
		}
		cout << endl; 
	}
	*/

	indMap = Graph2.reorderGraph(indMap);
	/*
	cout << "graph 2 after" << endl;
	for (int i = 0; i < Graph2.getNumNodes(); i++)
	{
		std::vector<int> c = Graph2.getChildren(i);
		for (int j = 0; j < c.size(); j++)
		{
			cout << c[j] << " ";
		}
		cout << endl; 
	}

	cout << "Graph2 Index Map" << endl; 
	for (int i = 0; i < indMap.size(); i++)
	{
		cout << indMap[i] << endl;
	}

        cout << " --- Child Match list Graph 1 --- " << endl;
        for( int i=0; i<Graph1.getNumNodes(); i++){
          cout << Graph1.getNodeMatch(i) << endl;
        }

	cout << "Chil2Parent Graph1 Before" << endl;
	for (int i = 0; i < Graph1.getNumNodes(); i++)
	{
		std::vector<int> C = Graph1.getChildren(i);
		for (int j = 0; j < C.size(); j++)
		{
			cout << C[j] << " ";
		}
		cout << endl; 
	}
	*/

	indMap = Graph1.reorderGraph(indMap);
	/*
	cout << "graph 1 after" << endl;
	for (int i = 0; i < Graph1.getNumNodes(); i++)
	{
		std::vector<int> c = Graph1.getChildren(i);
		for (int j = 0; j < c.size(); j++)
		{
			cout << c[j] << " ";
		}
		cout << endl; 
	}

	cout << "Graph1 Index Map" << endl; 
	for (int i = 0; i < indMap.size(); i++)
	{
		cout << indMap[i] << endl;
	}
        cout << " --- Child Match list Complex --- " << endl;
        for( int i=0; i<complex_graph.getNumNodes(); i++){
          cout << complex_graph.getNodeMatch(i) << endl;
        }

	cout << "Complex Graph Before" << endl;
	for (int i = 0; i < complex_graph.getNumNodes(); i++)
	{
		std::vector<int> c = complex_graph.getChildren(i);
		for (int j = 0; j < c.size(); j++)
		{
			cout << c[j] << " ";
		}
		cout << endl; 
	}
	*/
	indMap = complex_graph.reorderGraph(indMap);

	/*
	cout << "Complex Graph After" << endl;
	for (int i = 0; i < complex_graph.getNumNodes(); i++)
	{
		std::vector<int> c = complex_graph.getChildren(i);
		for (int j = 0; j < c.size(); j++)
		{
			cout << c[j] << " ";
		}
		cout << endl; 
	}


	cout << "End Index Map" << endl; 
	for (int i = 0; i < indMap.size(); i++)
	{
		cout << indMap[i] << endl;
	}


        cout << " --- Child Match list --- " << endl;
        for( int i=0; i<complex_graph.getNumNodes(); i++){
          cout << complex_graph.getNodeMatch(i) << endl;
        }
	cout << "Chil2Parent Reorder" << endl;
	for (int i = 0; i < coarsen1.getNumNodes(); i++)
	{
		std::vector<int> C = coarsen1.getChildren(i);
		for (int j = 0; j < C.size(); j++)
		{
			cout << C[j] << " ";
		}
		cout << endl; 
	}

	cout << "Index Map" << endl; 
	for (int i = 0; i < indMap.size(); i++)
	{
		cout << indMap[i] << endl;
	}

	cout << "Chil2Parent COmplex" << endl;
	for (int i = 0; i < complex_graph.getNumNodes(); i++)
	{
		std::vector<int> C = complex_graph.getChildren(i);
		for (int j = 0; j < C.size(); j++)
		{
			cout << C[j] << " ";
		}
		cout << endl; 
	}

	*/

//	indMap = complex_graph.reorderGraph(indMap);

	
/*	cout << "Chil2Parent Reorder COmplex" << endl;
	for (int i = 0; i < complex_graph.getNumNodes(); i++)
	{
		std::vector<int> C = complex_graph.getChildren(i);
		for (int j = 0; j < C.size(); j++)
		{
			cout << C[j] << " ";
		}
		cout << endl; 
	}


	cout << "Index Map" << endl; 
	for (int i = 0; i < indMap.size(); i++)
	{
		cout << indMap[i] << endl;
	}

	cout << "Reordered graph" << endl;

	for (int i = 0; i < complex_graph.getNumEdges(); i++)
	{
		cout << complex_graph.getEdgePoint(i,0) << " " << complex_graph.getEdgePoint(i,1) << endl;
	}
*/
	
	return 0;
}
