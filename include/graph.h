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
		Graph(std::string filename);
		double getEdgePoint(int i,int j);
		int getNumNodes();
		int getNumEdges();

		CSC_MATRIX computeGraphLaplacian();
		CSC_MATRIX  computeAdjacencyMatrix();
	private:
		int numEdges, numNodes;
		std::vector<std::vector<int> > edge;
		std::list<vector<int> > 
};


#endif
