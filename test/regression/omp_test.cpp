#include<iostream>
#include<iomanip>
#include<math.h>
#include<ctime>
#include<vector>
#include<omp.h>
#include"spectralBisection.h"
#include"graph.h"
#include"coarsen.h"
#include "arlsmat.h"
#include "areig.h"
#include "arlssym.h"

int runCoarsenTest();

using namespace std;

int ompTest()
{
	cout << "----------------------------------------------------------------------" << endl; 
	cout << "Running OpenMP Test" << endl; 
	cout << "----------------------------------------------------------------------" << endl; 

        int numThreads = 1;     
        cout << "Setting num_threads to " << numThreads << endl;
        omp_set_num_threads(numThreads);

        cout << "Rerunning coarsen test with multiple threads ... " << endl;        
        
        if(!runCoarsenTest())
		cout << "\033[0;32mPASSED: Coarsening Test\033[0m"  << endl << endl;
	else
		cout << "\033[0;31mFAILED: Coarsening Test\033[0m" << endl << endl;


	Graph complex_graph("complex_graph.dat");

	Graph Graph1; 
        std::vector<int> sbIndMap, g1IndMap;
        std::vector<std::vector<double> > timeKeeper(5,vector<double>(0));

        // Set known values
        sbIndMap = {2,3,4,0,1};
        g1IndMap = {3,6,4,5,7,8,0,9,1,2};

        // --- Do the coarsening
	Graph1.coarsenFrom(complex_graph,timeKeeper);

	std::vector<int> indMap = spectralBisection(Graph1);
	
        // Check spectral bisection index map
        if( indMap.size() != sbIndMap.size()){
          cout << "ERROR: Spectral Bisection Index map size incorrect" << endl;
          return 1;
        }       

	for (unsigned int i = 0; i < indMap.size(); i++){
          if(indMap[i] != sbIndMap[i]){
            cout << "ERROR: Element " << i << " incorrect in spectral bisection index map" << endl;
            cout << " -- Correct SB Index Map -- " << endl;
            for(unsigned int j=0; j<sbIndMap.size(); j++){
              cout << sbIndMap[j] << endl;
            }
            cout << " -- Computed SB Index Map -- " << endl;
            for(unsigned int j=0; j<indMap.size(); j++){
              cout << indMap[j] << endl;
            }
            return 1;   	
          }
        }

        cout << "Successfully computed spectral bisection index map" << endl;


	indMap = Graph1.reorderGraph(indMap);
	

	// Check level 1
        if( indMap.size() != g1IndMap.size()){
          cout << "ERROR: Spectral Bisection Index map size incorrect" << endl;
          return 1;
        }       

	for (unsigned int i = 0; i < indMap.size(); i++){
          if(indMap[i] != g1IndMap[i]){
            cout << "ERROR: Element " << i << " incorrect in G1 index map" << endl;
            cout << " -- Correct G1 Index Map -- " << endl;
            for(unsigned int j=0; j<g1IndMap.size(); j++){
              cout << g1IndMap[j] << endl;
            }
            cout << " -- Computed G1 Index Map -- " << endl;
            for(unsigned int j=0; j<indMap.size(); j++){
              cout << indMap[j] << endl;
            }
            return 1;   	
          }
        }
        
        cout << "Successfully computed G1 index map" << endl;

	return 0;
}
