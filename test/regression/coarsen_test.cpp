#include<iostream>
#include<iomanip>
#include<math.h>
#include<ctime>
#include<vector>
#include"graph.h"
#include"coarsen.h"

using namespace std;

int runCoarsenTest()
{
	cout << "----------------------------------------------------------------------" << endl; 
	cout << "Running Graph Coarsening Test" << endl; 
	cout << "----------------------------------------------------------------------" << endl; 

	Graph coarsen_me("coarsen_test.dat");
	Graph* ptr = &coarsen_me;

        cout << " --- Test matrix read ..." << endl;

        //Zeroth order print check
        cout << "Edge list " << endl;
        for(int i=0; i<coarsen_me.getNumEdges();i++){
          cout << coarsen_me.getEdgePoint(i,0) << " " << coarsen_me.getEdgePoint(i,1) << endl;
        }
        
        vector<int> colorList(coarsen_me.getNumNodes(), -1);
	std::clock_t start;
	double duration;

        cout << " --- Coloring ... " << endl;
	start=std::clock();

        colorGraph_shared(ptr, colorList);

	duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
        cout << " --- Coloring done, time: " << duration << endl;

        cout << " --- Color list --- " << endl << endl;
        for( int i=0; i<coarsen_me.getNumNodes(); i++)
          cout << "Node: " << i << "    Color: " << colorList[i] << endl;

        for( int i=0; i<coarsen_me.getNumNodes(); i++){
          for( unsigned int j=0; j<coarsen_me.getNeighbors(i).size(); j++){
            if( colorList[i] == colorList[coarsen_me.getNeighbors(i)[j]] ){
              cout << "ERROR: Node " << i << " and " << coarsen_me.getNeighbors(i)[j] << " have the same color and are neighbors. " << endl;
              return 1;
            }
          }
        }

        return 0;

}
        

