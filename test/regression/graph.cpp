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

CSC_MATRIX Graph::computeGraphLaplacian(CSC_MATRIX adjMat)
{
	CSC_MATRIX lapMat; 
	lapMat.n = numNodes; 
	lapMat.nnz = numEdges + numNodes; 

	lapMat.vals.resize(numEdges+numNodes,-1);
	lapMat.irow.resize(lapMat.vals.size());
	lapMat.pcol.resize(lapMat.n+1);

	for (size_t i = 0; i < lapMat.pcol.size(); i++)
	{
		lapMat.pcol[i] = adjMat.pcol[i]+i; 
	}

	
	lapMat.irow[0]=0;
	for (size_t i = 0; i < adjMat.pcol.size()-1; i++)
	{
		for (int j = adjMat.pcol[i]; j < adjMat.pcol[i+1]; j++)
		{
			lapMat.irow[j+i+1] = adjMat.irow[j];
		}
	}
	std::vector<int> NE;

	for (size_t i = 0; i< lapMat.pcol.size()-1; i++)
	{
		NE = getNeighbors(i);
		lapMat.vals[lapMat.pcol[i]]=NE.size();
		lapMat.irow[lapMat.pcol[i]]=i;

	}

	return lapMat;
}

CSC_MATRIX Graph::computeAdjacencyMatrix()
{ 
	CSC_MATRIX adjMat;
	adjMat.n = numNodes;
	adjMat.nnz = numEdges;

	adjMat.vals.resize(adjMat.nnz,1);
	adjMat.irow.resize(adjMat.nnz);
	adjMat.pcol.resize(adjMat.n+1);
	int numCount = 0; 
	int pcolInd = 1;

	adjMat.pcol[0]=0;
	adjMat.pcol[adjMat.n]=adjMat.nnz;
	for(int i = 0; i < numEdges; i++)
	{
		adjMat.irow[i]=edge[i][1];
	}

	std::vector<int> NE; 
	for (int i = 0; i < numNodes; i++)
	{
		NE = neighborList[i];
		for (size_t j = 0; j < NE.size(); j++)
		{
			if (NE[j] > i)
				numCount++; 
		}
		adjMat.pcol[pcolInd]=adjMat.pcol[pcolInd-1]+numCount;
		pcolInd++; 
		numCount= 0; 
	}

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