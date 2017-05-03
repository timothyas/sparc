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
        int numColors;
        vector<int> colorList(Graph1.getNumNodes(), -1);
        colorGraph_shared(Graph1, colorList, numColors);
        mxm_shared(Graph1, colorList, numColors); 
	Graph2.coarsenFrom(Graph1);

	Graph Graph3;
        colorGraph_shared(Graph2, colorList, numColors);
        mxm_shared(Graph2, colorList, numColors); 
	Graph3.coarsenFrom(Graph2);

	CSC_MATRIX adj = Graph3.computeAdjacencyMatrix();
	CSC_MATRIX lap = Graph3.computeGraphLaplacian(adj);

	ARluSymMatrix<double> L(lap.n,lap.nnz,&lap.vals[0],&lap.irow[0],&lap.pcol[0],'L');
	int nev = 2; 
	int ncv = min(2*nev+1,lap.n -1);
	ARluSymStdEig<double> prob(nev,L,"SA",ncv,0,100000);
	prob.FindEigenvectors();

	double	start = std::clock();
	double * Eigvec = prob.RawEigenvector(1);
	double * EigVal = prob.RawEigenvalues();
	cout << "2nd eigenvalue: " << EigVal[1] << endl;
	double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	cout << setprecision(15) << "--->Time: " << duration << endl;

	start = std::clock();
	vector<double> Eigvec2(Eigvec,Eigvec+lap.n);
	cout << "Getting index Map" << endl;
	std::vector<int> indMap =  getIndexMap(Eigvec2);
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	cout << setprecision(15) << "--->Time: " << duration << endl;

	indMap = Graph3.reorderGraph(indMap);
	indMap = Graph2.reorderGraph(indMap);
	indMap = Graph1.reorderGraph(indMap);

	adj = Graph1.computeAdjacencyMatrix();

	saveMatrixToFile(adj,"Results.dat");
	return 0;
}
