#ifndef GRAPH_H
#define GRAPH_H

#include<Eigen/Dense>
#include<vector>

using namespace Eigen;
using namespace std;
class Graph { 
	public: 
		Graph(string filename);
		double getEdgePoint(int i,int j);
		int getNumNodes();
		int getNumEdges();

		MatrixXd computeGraphLaplacian();
		MatrixXd computeAdjacencyMatrix();
	private:
		int numEdge, numNodes;
		MatrixXd edge; 
};

#endif
