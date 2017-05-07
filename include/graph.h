#ifndef GRAPH_H
#define GRAPH_H
#include<string>
#include<vector>


struct CSC_MATRIX {

	int n; //dimension of matrix
	int nnz; //number of nonzero elements
	std::vector<int> irow; //row index of all non zero elements
	std::vector<int> pcol; //pointer to beginning of each column
	std::vector<double> vals; //nonzero elements of matrix
};

class Graph { 

	public: 
		Graph();
		Graph(std::string filename);
		Graph(int numNodes, int numEdges);
		double getEdgePoint(int i,int j);
		int getNumNodes();
		int getNumEdges();
                int getNodeWeight(int node);
                int getEdgeWeight(int node, int neighborInd);
                int getNodeMatch(int node);
                void setNodeMatch(int node, int val);
                std::vector<int> getMatchList();
                std::vector<int> getNeighbors(int node);
                std::vector<int> getEdges(int node);
		std::vector<int> reorderGraph(std::vector<int> indMap);
                std::vector<int> getChildren(int node);
		int coarsenFrom(Graph & g);
                int writeEdgeList(std::string filename);
		void sortNeighborList();

		CSC_MATRIX computeGraphLaplacian(CSC_MATRIX adj);
		CSC_MATRIX  computeAdjacencyMatrix();
	private:
		int numEdges, numNodes, numChildren;
		std::vector<std::vector<int> > edge;
		std::vector<std::vector<int> > neighborList;
                std::vector<std::vector<int> > edgeWeights;
                std::vector<int> nodeWeights;
                std::vector<int> matchList;
                std::vector<std::vector<int> > parentList;
                std::vector<int> child2Parent;
};
#endif
