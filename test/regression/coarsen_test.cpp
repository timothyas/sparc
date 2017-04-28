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

	Graph coarsen_me("complex_graph.dat");
	Graph* ptr = &coarsen_me;

        //Zeroth order print check
        //cout << " --- Test matrix read ..." << endl;
        //cout << "Edge list " << endl;
        //for(int i=0; i<coarsen_me.getNumEdges();i++){
        //  cout << coarsen_me.getEdgePoint(i,0) << " " << coarsen_me.getEdgePoint(i,1) << endl;
        //}
        
        int numColors;
        vector<int> colorList(coarsen_me.getNumNodes(), -1);
        vector<int> nodeWeight(coarsen_me.getNumNodes(), 1);
        vector<int> matchList(coarsen_me.getNumNodes(),-1);
	std::clock_t start;
	double duration;


        // --- Color Test
        cout << " Coloring ... " << endl;
	start=std::clock();

        colorGraph_shared(ptr, colorList, numColors);

	duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
        cout << " Coloring done, time: " << duration << endl;

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

        // --- Maximal matching test
        cout << " Maximal matching ... " << endl;

        start=std::clock();
        mxm_shared(ptr, colorList, numColors, nodeWeight, matchList); 
        duration=(std::clock()-start) / (double) CLOCKS_PER_SEC;

        cout << " Matching done, time: " << duration << endl;

        cout << " --- Match list --- " << endl;
        for( int i=0; i<coarsen_me.getNumNodes(); i++){
          cout << matchList[i] << endl;
        }

        return 0;

}
        

