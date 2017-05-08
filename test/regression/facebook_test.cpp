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
#include"matrixOperations.h"
using namespace std;

int facebook()
{
	cout << "----------------------------------------------------------------------" << endl; 
	cout << "Running Facebook Test" << endl; 
	cout << "----------------------------------------------------------------------" << endl; 

        vector<double> timeKeeper(4,0.0);
	Graph Graph1("../../data/facebook_combined.txt");
	Graph Graph2;
	Graph Graph3;
	Graph Graph4;

	cout << "Coarsening level 1" << endl; 
	Graph2.coarsenFrom(Graph1,timeKeeper);

	cout << "Coarsening level 2" << endl; 
	Graph3.coarsenFrom(Graph2,timeKeeper);

	cout << "Coarsening level 3" << endl; 
	Graph4.coarsenFrom(Graph3,timeKeeper);

	cout << "Spectral Bisection" << endl; 
	std::vector<int> indMap = spectralBisection(Graph3);

	cout << "Uncoarsening"<< endl;

        //indMap = Graph4.reorderGraph(indMap);
	indMap = Graph3.reorderGraph(indMap);
	indMap = Graph2.reorderGraph(indMap);
	Graph1.reorderGraph(indMap);

	CSC_MATRIX adj = Graph1.computeAdjacencyMatrix();
	std::string outFile= "Results.dat";

	//saveMatrixToFile(adj,outFile);
	return 0;
}
