#include<iostream>
#include<iomanip>
#include<math.h>
#include<ctime>
#include<vector>
#include"graph.h"
#include"coarsen.h"
#include "arlsmat.h"
#include "areig.h"
#include "arlssym.h"
#include "spectralBisection.h"
using namespace std;

int facebook()
{
	cout << "----------------------------------------------------------------------" << endl; 
	cout << "Running Facebook Test" << endl; 
	cout << "----------------------------------------------------------------------" << endl; 

	Graph Graph1("../../data/facebook_combined.txt");
	Graph Graph2;
	Graph Graph3;
	Graph Graph4;

	cout << "Coarsening level 1" << endl; 
	Graph2.coarsenFrom(Graph1);

	cout << "Coarsening level 2" << endl; 
	Graph3.coarsenFrom(Graph2);

	cout << "Coarsening level 3" << endl; 
	Graph4.coarsenFrom(Graph3);

	cout << "Spectral Bisection" << endl; 
	std::vector<int> indMap = spectralBisection(&Graph4);

	cout << "Uncoarsening"<< endl;

        indMap = Graph4.reorderGraph(indMap);
	indMap = Graph3.reorderGraph(indMap);
	indMap = Graph2.reorderGraph(indMap);
	Graph1.reorderGraph(indMap);

	CSC_MATRIX adj = Graph1.computeAdjacencyMatrix();

	saveMatrixToFile(adj,"Results.dat");
	return 0;
}
