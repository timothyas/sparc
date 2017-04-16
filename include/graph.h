#ifndef GRAPH_H
#define GRAPH_H

#include<Eigen/Dense>

using namespace Eigen;
using namespace std;
class Graph { 
	public: 
		Graph(string filename);
		double getEdgePoint(int i,int j);
		double getNumNodes();
		double getNumEdges();

		MatrixXd computeGraphLaplacian();
		MatrixXd computeAdjacencyMatrix();
	private:
		int numEdge, numNodes;
		MatrixXd edge; 
};

#endif
