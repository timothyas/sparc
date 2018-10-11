/**
 * \file
 * \author Tim Smith and Gopal Yalla
 * 
 * \brief Defines class for a graph object and struct for a matrix stored in compact sparse column format. See CSC_MATRIX and Graph class for more information. 
 *
 */
#ifndef GRAPH_H
#define GRAPH_H
#include<string>
#include<vector>


/**
 * \brief Struct defining matrix in Compact Sparse Column (CSC) format
 *
 * \htmlonly
 * <IMG SRC="csc_storage.png" width=600px height=500px align="middle"/>
 * \endhtmlonly
 *
 */
struct CSC_MATRIX {

	int n; /**dimension of matrix*/
	int nnz; /**number of nonzero elements*/
	std::vector<int> irow; /**row index of all non zero elements*/
	std::vector<int> pcol; /**pointer to beginning of each column*/
	std::vector<double> vals; /**nonzero elements of matrix*/
};

/**
 * \brief 
 *
 * TODO: Give description of Graph class and add comments to each function
 *
 */
class Graph { 

	public: 
		Graph();
		Graph(std::string filename);
		Graph(int numNodes, int numEdges);
		int getEdgePoint(int i,int j);
		int getNumNodes();
		int getNumEdges();
                int getNodeWeight(int node);
                double getEdgeWeight(int node, int neighborInd);
                int getNodeMatch(int node);
                void setNodeMatch(int node, int val);
                std::vector<int> getMatchList();
                std::vector<int> getNeighbors(int node);
                std::vector<double> getEdges(int node);
		std::vector<int> reorderGraph(std::vector<int> indMap);
                std::vector<int> getChildren(int node);
		int coarsenFrom(Graph & g, std::vector<std::vector<double> >& timeKeeper);
                int writeEdgeList(std::string filename);
		void sortNeighborList();

		CSC_MATRIX computeGraphLaplacian(CSC_MATRIX adj);
		CSC_MATRIX  computeAdjacencyMatrix();
	private:
		int numEdges, numNodes, numChildren;
		std::vector<std::vector<int> > edge;
		std::vector<std::vector<int> > neighborList;
                std::vector<std::vector<double> > edgeWeights;
                std::vector<int> nodeWeights;
                std::vector<int> matchList;
                std::vector<std::vector<int> > parentList;
                std::vector<int> child2Parent;
};
#endif
