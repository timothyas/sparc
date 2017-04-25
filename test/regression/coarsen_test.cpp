#include<iostream>
#include<iomanip>
#include<math.h>
#include<ctime>
#include"graph.h"
#include"coarsen.h"

using namespace std;

int runCoarsenTest()
{
	cout << "----------------------------------------------------------------------" << endl; 
	cout << "Running Graph Coarsening Test" << endl; 
	cout << "----------------------------------------------------------------------" << endl; 

	Graph coarsen_me("coarsen.dat");
	Graph* ptr = &coarsen_me;

        cout << " --- Test matrix read, coloring ..." << endl;
        
        vector<int> colorList(coarsen_me.getNumNodes(), -1);
	std::clock_t start;
	double duration;

	start=std::clock();

        colorGraph_shared(ptr, colorList);

	duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
        cout << " --- Coloring done, time: " << duration << endl;

        cout << " --- Color list --- " << endl << endl;

        for( int i=0; i<coarsen_me.getNumNodes(); i++)
          cout << "Node: " << i << "    Color: " << colorList[i] << endl;

        return 0;

}
        

