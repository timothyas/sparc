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

using namespace std;

int ompTest()
{
	cout << "----------------------------------------------------------------------" << endl; 
	cout << "Running OpenMP Test" << endl; 
	cout << "----------------------------------------------------------------------" << endl; 



	Graph complex_graph("complex_graph.dat");

	Graph Graph1; 
	Graph Graph2;


        int numThreads = 1;     
        std::vector<int> sbIndMap, g2IndMap, g1IndMap;

        // Set known values
        sbIndMap = {0,3,1,2};
        g2IndMap = {0,1,6,2,3,4,5};
        g1IndMap = {0,1,2,9,3,4,5,6,7,8};


        cout << "Setting num_threads to " << numThreads << endl;
        omp_set_num_threads(numThreads);

        // Do the coarsening
	Graph1.coarsenFrom(complex_graph);
	Graph2.coarsenFrom(Graph1);

	std::vector<int> indMap = spectralBisection(Graph2);
	
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

	// Check level 2
	indMap = Graph2.reorderGraph(indMap);
	
        if( indMap.size() != g2IndMap.size()){
          cout << "ERROR: Index map 2->1 size incorrect" << endl;
          return 1;          
        }       

	for (unsigned int i = 0; i < indMap.size(); i++){
          if(indMap[i] != g2IndMap[i]){
            cout << "ERROR: Element " << i << " incorrect in graph 2->1 index map" << endl;
            cout << " -- Correct g2 Index Map -- " << endl;
            for(unsigned int j=0; j<g2IndMap.size(); j++){
              cout << g2IndMap[j] << endl;
            }
            cout << " -- Computed g2 Index Map -- " << endl;
            for(unsigned int j=0; j<indMap.size(); j++){
              cout << indMap[j] << endl;
            }
            return 1;   	
	  }
        }

        cout << "Successfully computed graph 2 index map" << endl;



	// Check level 1
	indMap = Graph1.reorderGraph(indMap);
	
        if( indMap.size() != g1IndMap.size()){
          cout << "ERROR: Index map 1->complex graph size incorrect" << endl;
          return 1;
        }       
        
	for (unsigned int i = 0; i < indMap.size(); i++){
          if(indMap[i] != g1IndMap[i]){
            cout << "ERROR: Element " << i << " incorrect in graph 1->complex graph index map" << endl;
            cout << " -- Correct g1 Index Map -- " << endl;
            for(unsigned int j=0; j<g1IndMap.size(); j++){
              cout << g1IndMap[j] << endl;
            }
            cout << " -- Computed g1 Index Map -- " << endl;
            for(unsigned int j=0; j<indMap.size(); j++){
              cout << indMap[j] << endl;
            }
            return 1;
          }
        }

        cout << "Successfully computed graph 1 index map" << endl;
	
        // Check complex graph 
	indMap = complex_graph.reorderGraph(indMap);

	return 0;
}
