#include"graph.h"
#include<omp.h>
#include<fstream>
#include<string>
#include<iostream>
#include<vector>
#include<algorithm>
#include"coarsen.h"
#include<exception>
#include<assert.h>
#include<stdexcept>

using namespace std; 

std::vector<int> Graph::reorderGraph(std::vector<int> indMap)
{
        std::vector<int> magicMap(numNodes,0);

        
        for(unsigned int i=0; i<numNodes; i++){
          for(unsigned int j=0; j<numNodes; j++){
            if( indMap[j]==i )
              magicMap[i]=j;
          }
        }

	neighborList.clear();
	neighborList.resize(numNodes,vector<int>(0));
	cout << " - index map " << endl;
	for (int i = 0; i < indMap.size(); i++)
	{
		cout << indMap[i] << endl;
	}
		

	cout << "-- before reordering " << endl;

	for (int i = 0; i < numEdges; i++)
	{
		cout << edge[i][0] << " " << edge[i][1] << endl;
	}
	for (int i = 0; i < numEdges; i++)
	{
		edge[i][0] = magicMap[edge[i][0]];
		edge[i][1] = magicMap[edge[i][1]];
		neighborList[edge[i][0]].push_back(edge[i][1]);
		neighborList[edge[i][1]].push_back(edge[i][0]);
	}
	cout << "-- after reordering " << endl;

	for (int i = 0; i < numEdges; i++)
	{
		cout << edge[i][0] << " " << edge[i][1] << endl;
	}

	std::vector<std::vector<int> > tempParentList; //(parentList); 
	for (unsigned int i = 0; i < parentList.size(); i++)
	{
                tempParentList.push_back(parentList[indMap[i]]);
		//tempParentList[i] = std::move(parentList[indMap[i]]);
	}
	parentList = std::move(tempParentList); 

	std::vector<int> childIndMap (numChildren,0);
	int count = 0; 
	for (unsigned int i = 0; i < parentList.size();i++)
	{
		for(unsigned int j = 0; j < parentList[i].size(); j++)
		{
			childIndMap[count] = parentList[i][j];
			count ++; 
		}
	}
	return childIndMap; 
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

int Graph::getNodeWeight(int node)
{
        return nodeWeights[node];
}

int Graph::getEdgeWeight(int node, int neighborInd)
{
        return edgeWeights[node][neighborInd];
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

std::vector<int> Graph::getEdges(int node)
{
        return edgeWeights[node];
}

std::vector<int> Graph::getChildren(int node)
{
        return parentList[node];
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

	//need to sort new neighborList in order to get
	//the appropriate irow
//	#pragma omp parallel for
	for (int i = 0; i < numNodes; i++)
	{
		std::sort(neighborList[i].begin(),neighborList[i].end());
	}


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

        nodeWeights.resize(numNodes,1);
        matchList.resize(numNodes,-1);
	edge.resize(numEdges,vector<int>(2));
	neighborList.resize(numNodes,vector<int>(0));
        edgeWeights.resize(numNodes,vector<int>(0));

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
                edgeWeights[edge1].push_back(1);
                edgeWeights[edge2].push_back(1);
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

	numChildren  = numNodes; 
	parentList.resize(numNodes,std::vector<int>(0));
	for (unsigned int i = 0; i < parentList.size(); i++)
	{
		parentList[i].push_back(i);
	}
}


Graph::Graph()
{
}
int Graph::coarsenFrom(Graph & g) 
{

        // Perform coloring, then  maximal matching
        int numColors;
        vector<int> colorList(g.getNumNodes(),-1);
        
        colorGraph_shared(g, colorList, numColors);
        mxm_shared(g, colorList, numColors);

        numChildren = g.getNumNodes();
        
        // Find number of nodes for coarsened graph
        vector<int> unmatched;
        selectUnmatched_shared(g.getMatchList(),-1,unmatched);
        numNodes = (g.getNumNodes() - (int)unmatched.size()) / 2 + (int)unmatched.size();

        if( (g.getNumNodes() - (int)unmatched.size()) % 2 != 0 ) {
          cout << "Error: Creating coarse graph, bad math for numNodes. See Graph::coarsenFrom(Graph&) " << endl;
          return 1;
        }

        // Create parentList and sum node weights
        /* Note: 
         *       parentList maps parent node number -> vector containing child nodes
         *       child2Parent maps child node number -> parent node
         */

        vector<int> tempMatchList(g.getMatchList());
        parentList.resize(numNodes,vector<int>(0));
        nodeWeights.resize(numNodes);
        child2Parent.resize(g.getNumNodes());
        matchList.resize(numNodes,-1);

        unsigned int k=0;        
        for( unsigned int i=0; i<tempMatchList.size(); i++){

          if( tempMatchList[i] == -1 ){

            parentList[k].push_back(i);
            nodeWeights[k] = g.getNodeWeight(i);
            child2Parent[i] = k;
            k++;
          }
          else if( tempMatchList[i] != -2 ){

            parentList[k].push_back(i);
            parentList[k].push_back(tempMatchList[i]);
            nodeWeights[k] = g.getNodeWeight(i)+g.getNodeWeight(tempMatchList[i]);
        
            child2Parent[i] = k;
            child2Parent[tempMatchList[i]] = k;

            tempMatchList[tempMatchList[i]]=-2;
            tempMatchList[i]=-2;

            k++;
          } 
        }

        // Create edge and neighborlist
        neighborList.resize(numNodes,vector<int>(0));
        edgeWeights.resize(numNodes,vector<int>(0));

        bool mappedToSameNode; 
        int currentChild, currentChildNeighbor, neighborInd, neighborInd2;
        vector<int> tempEdge(2);

        for( int i=0; i<numNodes; i++){

          for( unsigned int j=0; j<parentList[i].size(); j++){
            
            //current parent = i
            currentChild=parentList[i][j];

            for( k=0; k<g.getNeighbors(currentChild).size(); k++){ 

              currentChildNeighbor = g.getNeighbors(currentChild)[k];

              if( currentChildNeighbor > currentChild ){

                // Check if child neighbors map to same node
                mappedToSameNode = child2Parent[currentChild] == child2Parent[currentChildNeighbor];

                // Look for the neighbor in parent's neighbor list
                // to decide whether to add edge or just increment weight
                neighborInd=-1;
                for (unsigned int ii = 0; ii < neighborList[i].size(); ii++){
                  if (neighborList[i][ii] == child2Parent[currentChildNeighbor]){
                    neighborInd=ii;
                    break;
                  }
                }

                if( !mappedToSameNode && neighborInd == -1 ){
          
                  // Edge hasn't been accounted for
                  neighborList[i].push_back(child2Parent[currentChildNeighbor]);
                  neighborList[child2Parent[currentChildNeighbor]].push_back(i);

                  edgeWeights[i].push_back(g.getEdgeWeight(currentChild,k));
                  edgeWeights[child2Parent[currentChildNeighbor]].push_back(g.getEdgeWeight(currentChild,k));

                  tempEdge[0] = child2Parent[currentChild];
                  tempEdge[1] = child2Parent[currentChildNeighbor];
                  edge.push_back(tempEdge);
                  
                }
                
                else if( !mappedToSameNode && neighborInd != -1 ){
                  
                  // Edge has been accounted for, increment weight
                  edgeWeights[i][neighborInd] += g.getEdgeWeight(currentChild,k);

                  // This seems expensive but I don't know how to do it better
                  neighborInd2=-1;
          	  for(unsigned int ii=0; ii<neighborList[child2Parent[currentChildNeighbor]].size(); ii++){
                    if(neighborList[child2Parent[currentChildNeighbor]][ii]==i){
                      neighborInd2=ii;
                      break;
                    }
                  }
                  //neighborLoc2 = find(tempList.begin(),tempList.end(), [&i](int ii){ return ii == i;} );
                  //neighborInd2 = distance(tempList.begin(),neighborLoc2);
                  
                  if( neighborInd2==-1 ){
                    cout << "Error: couldn't find parent in neighbor's neighbor list. This edge should already be accounted for. See Graph::coarsenFrom(Graph&)." << endl;
                    return 1;
                  }
                  
                  edgeWeights[neighborList[i][neighborInd]][neighborInd2] += g.getEdgeWeight(currentChild,k);
                }

              }// if neighbor > me
            }// end loop thru e/ child's neighbors
          }//end loop thru parent's children
        }//end loop thru parent nodes

        numEdges = edge.size();

	return 0; 
}
