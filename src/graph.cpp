#include"graph.h"
#include<fstream>
#include<string>
#include<iostream>
#include<vector>
#include<algorithm>

using namespace std; 


double Graph::getEdgePoint(int i,int j)
{
	return edge[i][j];
}

int Graph::getNumNodes()
{
	return numNodes;
}

int Graph::getNumEdges()
{
	return numEdges;
}

std::vector<int> Graph::getNeighbors(int i)
{
	return neighborList[i];
}

CSC_MATRIX Graph::computeGraphLaplacian()
{
	CSC_MATRIX lapMat; 
	lapMat.n = numNodes; 
	lapMat.nnz = numEdges + numNodes; 

	lapMat.vals.resize(numEdges+numNodes,-1);
	lapMat.irow.reserve(lapMat.vals.size());
	lapMat.pcol.reserve(lapMat.n+1);

	std::vector<double> degree (lapMat.n,0);
	int edge1; 
	int edge2; 
	int diag_count = 1; 
	int gap; 

	lapMat.pcol.push_back(0);
	lapMat.irow.push_back(0);
	int row_count = 1; 
	for (int i = 0; i < numEdges; i++)
	{
		edge1 = edge[i][0];
		edge2 = edge[i][1];
		degree[edge1]++;
		degree[edge2]++;
		if (i != 0 && edge1 != edge[i-1][0])
		{
			gap = edge1 - edge[i-1][0];
			for (int j = 0; j < gap; j++)
			{
				lapMat.pcol.push_back(i+diag_count+j);
				lapMat.irow.push_back(row_count+j);
			}
			diag_count+=gap; 
			row_count+=gap;
		}
		lapMat.irow.push_back(edge2);
	}
	gap = lapMat.n -1 - row_count; 
	for (int i = 0; i < gap; i++)
	{
		lapMat.pcol.push_back(diag_count + numEdges+ i);
		lapMat.irow.push_back(row_count+i);
	}
	lapMat.irow.push_back(lapMat.n-1);
	lapMat.pcol.push_back(lapMat.vals.size()-1);

	for (size_t i = 0; i < lapMat.pcol.size(); i++)
	{
		lapMat.vals[lapMat.pcol[i]] = degree[i];
	}

	lapMat.pcol.push_back(lapMat.nnz);
	return lapMat;
}

CSC_MATRIX Graph::computeAdjacencyMatrix()
{ 
	CSC_MATRIX adjMat;
	adjMat.n = numNodes;
	adjMat.nnz = numEdges;

	adjMat.vals.resize(adjMat.nnz,1);
	adjMat.irow.reserve(adjMat.nnz);
	adjMat.pcol.reserve(adjMat.n+1);
	int edge1 = -1; 
	int edge2 = -1;

	for(int i = 0; i < numEdges; i++)
	{
		edge1 = edge[i][0];
		edge2 = edge[i][1];
		adjMat.irow.push_back(edge2);
		if (i == 0)
			adjMat.pcol.push_back(i);
		else if( edge1 != edge[i-1][0] )
			adjMat.pcol.push_back(i);
	}

	adjMat.pcol.push_back(adjMat.nnz);
	return adjMat;
}


Graph::Graph(std::string filename)
{
	cout << "Initalizing graph from " << filename << endl;
	ifstream inFile; 
	inFile.open(filename.c_str());
	inFile >> numNodes >> numEdges;

	edge.resize(numEdges,vector<int>(2));

	neighborList.resize(numNodes,vector<int>(0));

	int edge1 = 0;
        int edge2 = 0;
	size_t row_counter=0; 

	inFile >> edge1 >> edge2; 
	while(!inFile.eof())
	{
		edge[row_counter][0]=edge1;
		edge[row_counter][1]=edge2;
		neighborList[edge1].push_back(edge2);
		neighborList[edge2].push_back(edge1);
		row_counter++; 
		inFile >> edge1 >> edge2; 
	}

	inFile.close();
}
