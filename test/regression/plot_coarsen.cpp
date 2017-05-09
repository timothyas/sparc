#include<iostream>
#include<iomanip>
#include<math.h>
#include<ctime>
#include<vector>
#include<omp.h>
#include"graph.h"
#include"coarsen.h"
#include "arlsmat.h"
#include "areig.h"
#include "arlssym.h"
#include "spectralBisection.h"
#include"matrixOperations.h"
using namespace std;

int plotCoarsen()
{
	cout << "----------------------------------------------------------------------" << endl; 
	cout << "Output for plotting coarsened graphs" << endl; 
	cout << "----------------------------------------------------------------------" << endl; 

        omp_set_num_threads(12);
        vector<vector<double> > timeKeeper(5,vector<double>(0));
	Graph Graph1("../../data/facebook_jumbled.txt");
	Graph Graph2;
	Graph Graph3;

        cout << "Writing facebook_jumbled.txt as csc " << endl;
        std::string fname="../../results/multi-coarsen-12-procs/adj_fb.dat";
        CSC_MATRIX adj=Graph1.computeAdjacencyMatrix();
        saveCSCMatrixToFile(adj,fname);

	cout << "Coarsening level 1" << endl; 
	Graph2.coarsenFrom(Graph1,timeKeeper);
        cout << "Writing 1c as csc " << endl;
        fname="../../results/multi-coarsen-12-procs/adj_c1.dat";
        adj=Graph2.computeAdjacencyMatrix();
        saveCSCMatrixToFile(adj,fname);

	cout << "Coarsening level 2" << endl; 
	Graph3.coarsenFrom(Graph2,timeKeeper);
        cout << "Writing 2c as csc " << endl;
        fname="../../results/multi-coarsen-12-procs/adj_c2.dat";
        adj=Graph3.computeAdjacencyMatrix();
        saveCSCMatrixToFile(adj,fname);


	cout << "Spectral Bisection" << endl; 
	std::vector<int> indMap = spectralBisection(Graph3);

	cout << "Uncoarsening"<< endl;

	indMap = Graph3.reorderGraph(indMap);
        cout << "Writing reordered 2c as csc " << endl;
        fname="../../results/multi-coarsen-12-procs/adj_c2_reorder.dat";
        adj=Graph3.computeAdjacencyMatrix();
        saveCSCMatrixToFile(adj,fname);

	indMap = Graph2.reorderGraph(indMap);
        cout << "Writing reordered 1c as csc " << endl;
        fname="../../results/multi-coarsen-12-procs/adj_c1_reorder.dat";
        adj=Graph2.computeAdjacencyMatrix();
        saveCSCMatrixToFile(adj,fname);

	Graph1.reorderGraph(indMap);
        cout << "Writing reordered fb as csc " << endl;
        fname="../../results/multi-coarsen-12-procs/adj_fb_reorder.dat";
        adj=Graph1.computeAdjacencyMatrix();
        saveCSCMatrixToFile(adj,fname);

	return 0;
}
