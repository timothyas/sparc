#include"graph.h"
#include<omp.h>
#include<fstream>
#include<string>
#include<iostream>
#include<vector>
#include<algorithm>
#include<exception>
#include<assert.h>
#include<stdexcept>


using namespace std; 

int Graph::reorderGraph(std::vector<int> indMap)
{
	for (int i = 0; i < numEdges; i++)
	{
		edge[i][0] = indMap[edge[i][0]];
		edge[i][1] = indMap[edge[i][1]];
	}

	neighborList.clear();
	neighborList.resize(numNodes,vector<int>(0));

	for (int i = 0; i < numEdges; i++)
	{
		neighborList[edge[i][0]].push_back(edge[i][1]);
		neighborList[edge[i][1]].push_back(edge[i][0]);
	}

	//need to sort new neighborList in order to get
	//the appropriate irow
	#pragma omp parallel for
	for (int i = 0; i < numNodes; i++)
	{
		std::sort(neighborList[i].begin(),neighborList[i].end());
	}

	return 0; 
}

double Graph::getEdgePoint(int i,int j)
{
	if (j > 1 || i > numEdges-1)
	{
		std::cout << "Error: Invalid edge. Must have j <= 1 & i <=numEdges-1" << endl;
		assert(j < 1);
		assert(i < numEdges);
	}
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

	// laplacian has all the points as the adjacency matrix but has 
	// points on the every diagonal. We can modify the adj pcol and irow 
	// to get the pcol, irow for the laplacian 

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
	adjMat.irow.reserve(adjMat.nnz);
	adjMat.pcol.resize(adjMat.n+1);
	int numCount = 0; 
	int pcolInd = 1;

	adjMat.pcol[0]=0;
	adjMat.pcol[adjMat.n]=adjMat.nnz;

	// we determine information from NeighborList because we can gaurentee if 
	// will be sorted. 
	std::vector<int> NE; 
	for (int i = 0; i < numNodes; i++)
	{
		NE = neighborList[i];
		for (size_t j = 0; j < NE.size(); j++)
		{
			if (NE[j] > i)
			{
				adjMat.irow.push_back(NE[j]);
				numCount++; 
			}
		}
		// pcol[0] = 0; 
		// pcol[i] = pcol[i-1] + nnz in (i-1) column 
		// pcol[n+1] should thus always be nnz  
		adjMat.pcol[pcolInd]=adjMat.pcol[pcolInd-1]+numCount;
		pcolInd++; 
		numCount= 0; 
	}

	return adjMat;
}

Graph::Graph(int nodes,int edges)
{
	numNodes = nodes; 
	numEdges = edges;
	edge.resize(edges,vector<int>(2));
	neighborList.resize(nodes,vector<int>(0));
}


Graph::Graph(std::string filename)
{
	int error = 0; // error checking for constructor 
	cout << "Initalizing graph from: " << filename << endl;
	ifstream inFile; 
	try
	{
		inFile.open(filename.c_str());
		if (!inFile.good())
		{
			error = 1; 
			throw std::invalid_argument( "Error: File does not exist.");
		}

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
	catch(std::exception & e)
	{
		cerr << e.what() << endl;
	}
	assert(error == 0);
}
