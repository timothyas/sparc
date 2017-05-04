#include<iostream>
#include<iomanip>
#include<math.h>
#include<ctime>
#include<vector>
#include"graph.h"
#include"spectralBisection.h"
using namespace std;

int test_getAij()
{
	//double value; 
	Graph complex_graph("complex_graph.dat");
	//Graph *ptr = &complex_graph;
	CSC_MATRIX adj = complex_graph.computeAdjacencyMatrix();



	return 0;
}


