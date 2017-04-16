#include"graph.h"
#include<Eigen/Dense>
#include<fstream>
#include<iostream>
using namespace Eigen; 
using namespace std; 

double Graph::getEdgePoint(int i,int j)
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

MatrixXd Graph::computeGraphLaplacian()
{

	MatrixXd L = MatrixXd::Zero(numNodes,numNodes);
	MatrixXd adjMat = this->computeAdjacencyMatrix();
	// Sum along columns to get degrees
	VectorXd partialSum = adjMat.colwise().sum();
	
	// Diagonal elements L = D-A
	for( size_t i = 0; i<numNodes; i++)
		L(i,i) = partialSum(i) - adjMat(i,i);

	// Off diagonals
	for( size_t i = 0; i<numNodes; i++)
	{
	  for( size_t j = i+1; j<numNodes; j++)
	  {
		L(i,j) = -adjMat(i,j);
		L(j,i) = -adjMat(j,i);
	  }
	}

	return L;
}

MatrixXd Graph::computeAdjacencyMatrix()
{ 
	int m,n;
	MatrixXd A = MatrixXd::Zero(numNodes,numNodes);

	// Adjacency mat gets 1 where there's a connection btwn 2 nodes
	// 	may need to replace with weights ...
	for( size_t i=0; i<this->getNumEdges(); i++)
	{
		m = (int)edge(i,0);
		n = (int)edge(i,1);
		
		A(m,n) = 1;
		A(n,m) = 1;
	}

	return A;
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
}


