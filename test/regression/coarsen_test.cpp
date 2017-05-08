#include<iostream>
#include<iomanip>
#include<math.h>
#include<sys/time.h>
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

        int numColors;
        vector<int> colorList(coarsen_me.getNumNodes(), -1);
	struct timeval start, end;
	double duration;
        vector<vector<double> > timeKeeper(5,vector<double>(0));


        // --- Color Test
	gettimeofday(&start,NULL);
        colorGraph_shared(coarsen_me, colorList, numColors);
	gettimeofday(&end,NULL);
	duration = ((end.tv_sec - start.tv_sec)*1000000u + end.tv_usec - start.tv_usec) / 1.e6;


        for( int i=0; i<coarsen_me.getNumNodes(); i++){
          for( unsigned int j=0; j<coarsen_me.getNeighbors(i).size(); j++){
            if( colorList[i] == colorList[coarsen_me.getNeighbors(i)[j]] ){

              cout << "COLORING ERROR: Node " << i << " and " << coarsen_me.getNeighbors(i)[j];
              cout << " have the same color and are neighbors. " << endl;

              cout << " --- Color list --- " << endl << endl;
              for( int i=0; i<coarsen_me.getNumNodes(); i++)
                cout << "Node: " << i << "    Color: " << colorList[i] << endl;

              return 1;
            }
          }
        }

        cout << "Successfully colored graph in " << duration << " sec" << endl;
        

        // --- Maximal matching test
	gettimeofday(&start,NULL);
        mxm_shared(coarsen_me, colorList, numColors);
	gettimeofday(&end,NULL);
	duration = ((end.tv_sec - start.tv_sec)*1000000u + end.tv_usec - start.tv_usec) / 1.e6;

        for( int i=0; i<coarsen_me.getNumNodes(); i++){
          for ( int j=i+1; j<coarsen_me.getNumNodes(); j++){
            if( coarsen_me.getNodeMatch(i) == coarsen_me.getNodeMatch(j) && coarsen_me.getNodeMatch(i) != -1 ){
              cout << "MATCHING ERROR: Found multiple node matching. " << endl;
              cout << " -- Nodes: " << i << " and " << j << " are trying to match " << coarsen_me.getNodeMatch(i) << endl;
            
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
        Graph parent;

	gettimeofday(&start,NULL);
	parent.coarsenFrom(coarsen_me,timeKeeper); 
	gettimeofday(&end,NULL);
	duration = ((end.tv_sec - start.tv_sec)*1000000u + end.tv_usec - start.tv_usec) / 1.e6;

        cout << "Successfully coarsened graph in " << duration << " sec" << endl;

        return 0;

}
        

