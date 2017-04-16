#include"graph.h"
#include<Eigen/Dense>
#include<fstream>
#include<iostream>
using namespace Eigen; 
using namespace std; 

double Graph::getEdgePoint(i,j)
{
	return edge(i,j);
}

double Graph::getNumNodes()
{
	return numNodes;
}

double Graph::getNumEdges()
{
	return numEdge;
}

Graph::Graph(string filename)
{
	ifstream inFile; 
	inFile.open(filename.c_str());
	size_t numEdge = 0; 
	string line;
	
	//Determine total number of edges
	while (getline(inFile,line))
	{
		++numEdge;
	}

	inFile.close(); 
	edge.resize(numEdge,2);

	double edge1,edge2;
	size_t row_counter=0; 

	numNodes = 0; 
	inFile.open(filename.c_str());
	inFile >> edge1 >> edge2; 
	while(!inFile.eof())
	{
		edge(row_counter,0) = edge1; 
		edge(row_counter,1) = edge2; 
		row_counter++; 
		inFile >> edge1 >> edge2; 
	}
	inFile.close();

	numNodes = edge.maxCoeff();
	cout << "Number of Nodes: " << numNodes << endl; 
	cout << "Number of Edges: " << edge.rows() << endl; 
}


