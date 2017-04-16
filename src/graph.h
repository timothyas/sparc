#ifndef GRAPH_H
#define GRAPH_H

#include<Eigen/Dense>

using namespace Eigen;
using namespace std;
class Graph { 
	public: 
		Graph(string filename);
	private:
		int numEdge, numNodes;
		MatrixXd edge; 
};

#endif
