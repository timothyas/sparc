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
                int getNodeWeight(int node);
                int getEdgeWeight(int node, int neighborInd);
                int getNodeMatch(int node);
                void setNodeMatch(int node, int val);
                std::vector<int> getMatchList();
                std::vector<int> getNeighbors(int node);

		CSC_MATRIX computeGraphLaplacian(CSC_MATRIX adj);
		CSC_MATRIX  computeAdjacencyMatrix();
	private:
		int numEdges, numNodes;
		std::vector<std::vector<int> > edge;
		std::vector<std::vector<int> > neighborList;
                std::vector<std::vector<int> > edgeWeights;
                std::vector<int> nodeWeights;
                std::vector<int> matchList;
};

class CoarseGraph : public Graph {

        public:
                CoarseGraph(const Graph& g) : child{g};

        private:
                Graph child;
                std::vector<std::vector<int> > parentList;
                std::vector<int> child2Parent;
};

#endif
