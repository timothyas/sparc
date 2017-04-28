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
	cout << "HERE" << endl;
	double value; 
	Graph complex_graph("complex_graph.dat");
	CSC_MATRIX adj = complex_graph.computeAdjacencyMatrix();

	for (int i = 0; i < adj.n; i++)
	{
		for (int j = 0; j < adj.n;i++)
		{
			cout << getAij(adj,i,j) <<  " ";
		}
		cout << endl;
	}

	return 0;
}


