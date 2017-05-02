#include"graph.h"
#include<fstream>
#include<string>
#include<iostream>
#include<vector>
#include<algorithm>
#include"coarsen.h"

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

int Graph::getNodeWeight(int node)
{
        return nodeWeight[node];
}

int Graph::getEdgeWeight(int node, int neighborInd)
{
        return edgeWeight[node][neighborInd];
}

int Graph::getNodeMatch(int node)
{
        return matchList[node];
}

void Graph::setNodeMatch(int node, int val)
{
        matchList[node] = val;
        return;
}

std::vector<int> Graph::getMatchList()
{
        return matchList;
}

std::vector<int> Graph::getNeighbors(int node)
{
	return neighborList[node];
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

        nodeWeight.resize(numNodes,1);
        matchList.resize(numNodes,-1);
	edge.resize(numEdges,vector<int>(2));
	neighborList.resize(numNodes,vector<int>(0));
        edgeWeight.resize(numNodes,vector<int>(0));

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
                edgeWeight[edge1].push_back(1);
                edgeWeight[edge2].push_back(1);
		row_counter++; 
		inFile >> edge1 >> edge2; 
	}

	inFile.close();
}

coarseGraph::coarseGraph(Graph* g) 
{

        // Find number of nodes for coarsened graph
        vector<int> unmatched;
        selectUnmatched_shared(g->getMatchList(),-1,unmatched);
        numNodes = (g->getNumNodes() - (int)unmatched.size()) / 2 + (int)unmatched.size();

        if( (g->getNumNodes() - (int)unmatched.size()) % 2 != 0 ) {
          cout << "Error: Creating coarse graph, bad math for numNodes ... " << endl;
          return 1;
        }

        // Create parentList and sum node weights
        /* Note: 
         *       parentList maps parent node number -> vector containing child nodes
         *       child2Parent maps child node number -> parent node
         */

        vector<int> tempMatchList=g->getMatchList;
        parentList.resize(numNodes,vector<int>(0));
        nodeWeights.resize(numNodes);
        child2Parent.resize(g->getNumNodes());
        matchList.resize(numNodes,-1);

        unsigned int k=0;        
        for( unsigned int i=0; i<tempMatchList.size(); i++){

          if( tempMatchList[i] == -1 ){

            parentList[k].push_back(i);
            nodeWeights[k] = g->getNodeWeight(i);
            child2Parent[i] = k;

            k++;
          }
          else if( tempMatchList[i] != -2 ){

            parentList[k].push_back(i);
            parentList[k].push_back(tempMatchList[i]);
            nodeWeights[k] = g->getNodeWeight(i)+g->getNodeWeight(tempMatchList[i]);
        
            child2Parent[i] = k;
            child2Parent[tempMatchList[i]] = k;

            tempMatchList[i]=-2;
            tempMatchList[tempMatchList[i]]=-2;
            k++;
          } 
        }

        // Create edge and neighborlist
        neighborList.resize(numNodes, vector<int>(0));
        edgeWeight.resize(numNodes,vector<int>(0));

        bool mappedToSameNode; 
        vector::iterator neighborLoc, neighborLoc2;
        int currentChild, currentChildNeighbor, neighborInd, neighborInd2;
        vector<int> tempEdge(2);


        for( int i=0; i<numNodes; i++){
          for( unsigned int j=0; j<parentList[i].size(); j++){
            
            //current parent = i
            currentChild=parentList[i][j];

            for( k=0; k<g->getNeighbors[currentChild].size(); k++){ 

              currentChildNeighbor = g->getNeighbors[currentChild][k];

              mappedToSameNode = child2Parent[currentChild] == child2Parent[currentChildNeighbor];
              neighborLoc = find(neighborList[i].begin(),neighborList[i].end(), \
                 [](int ii){ return ii == child2Parent[currentChildNeighbor];} );

              if( !mappedToSameNode && neighborLoc == neighborList[i].end() ){
        
                // Edge hasn't been accounted for
                neighborList[i].push_back(child2Parent[currentChildNeighbor]);
                neighborList[child2Parent[currentChildNeighbor]].push_back(i);

                edgeWeight[i].push_back(g->getEdgeWeight(currentChild,k));
                edgeWeight[child2Parent[currentChildNeighbor]].push_back(g->getEdgeWeight(currentChild,k));

                tempEdge[0] = child2Parent[currentChild];
                tempEdge[1] = child2Parent[currentChildNeighbor];
                edge.push_back(tempEdge);
                
              }
              
              else if( !mappedToSameNode && neighborLoc != neighborList[i].end() ){
                
                // Edge has been accounted for, increment weight
                neighborInd = distance(neighborList[i].begin(),neighborLoc);
                edgeWeight[i][neighborInd] += g->getEdgeWeight(currentChild,k);

                // This seems expensive but I don't know how to do it better
                neighborLoc2 = find(neighborList[neighborInd].begin(),neighborList[neighborInd].end(), \
                 [](int ii){ return ii == i;} );
                neighborInd2 = distance(neighborList[neighborInd].begin(),neighborLoc2);
                edgeWeight[neighborList[neighborInd]][neighborInd2] += g->getEdgeWeight(currentChild,k);
              }

            }// end loop thru e/ child's neighbors
          }//end loop thru parent's children
        }//end loop thru parent nodes

}
