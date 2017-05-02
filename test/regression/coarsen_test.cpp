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

        int numColors;
        vector<int> colorList(coarsen_me.getNumNodes(), -1);
	std::clock_t start;
	double duration;


        // --- Color Test
	start=std::clock();

        colorGraph_shared(ptr, colorList, numColors);

	duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;


        for( int i=0; i<coarsen_me.getNumNodes(); i++){
          for( unsigned int j=0; j<coarsen_me.getNeighbors(i).size(); j++){
            if( colorList[i] == colorList[coarsen_me.getNeighbors(i)[j]] ){

              cout << "COLORING ERROR: Node " << i << " and " << coarsen_me.getNeighbors(i)[j] << " have the same color and are neighbors. " << endl;

              cout << " --- Color list --- " << endl << endl;
              for( int i=0; i<coarsen_me.getNumNodes(); i++)
                cout << "Node: " << i << "    Color: " << colorList[i] << endl;


              return 1;
            }
          }
        }

        cout << "Successfully colored graph in " << duration << " sec" << endl;

        // --- Maximal matching test

        start=std::clock();
        mxm_shared(ptr, colorList, numColors); 
        duration=(std::clock()-start) / (double) CLOCKS_PER_SEC;

        for( int i=0; i<coarsen_me.getNumNodes(); i++){
          for ( int j=i+1; j<coarsen_me.getNumNodes(); j++){
            if( coarsen_me.getNodeMatch(i) == coarsen_me.getNodeMatch(j) && coarsen_me.getNodeMatch(i) != -1 ){
              cout << "MATCHING ERROR: Found multiple node matching. " << endl;
              cout << " -- Nodes: " << i << " and " << j << " are trying to match " << matchList[i] << endl;
            

              cout << " Matching done, time: " << duration << endl;

              cout << " --- Match list --- " << endl;
              for( int i=0; i<coarsen_me.getNumNodes(); i++){
                cout << coarsen_me.getNodeMatch(i) << endl;
              }
              return 1;
            }
          }
        }

        cout << "Successfully computed maximal matching in " << duration << " sec" << endl;

        // --- Create coarsened graph
        
        start=std::clock();
        coarseGraph parent(ptr);
        duration=(std::clock()-start) / (double) CLOCKS_PER_SEC;

        cout << " --- Child Match list --- " << endl;
        for( int i=0; i<coarsen_me.getNumNodes(); i++){
          cout << coarsen_me.getNodeMatch(i) << endl;
        }
        cout << " --- Coarse neighbor list --- " << endl;
        for( int i =0; i<parent.getNumNodes(); i++){
          cout << "node " << i << " : ";
          for( int j =0; j<parent.getNeighbors(i).size(); j++){
            cout << j << " ";
          }
          cout << endl;
        }

        cout << "Created coarse graph in " << duration << " sec" << endl;

        return 0;

}
        

