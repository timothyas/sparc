/* File containing routines for coarsening a large
 * sparse graph.
 */

#include<stdlib.h>
#include<math.h>
#include<vector>
#include<Eigen/Dense>
#include<omp.h>
#include"graph.h"
#include"coarsen.h"


#define THREADS 4

using namespace std;
using namespace Eigen; 

int colorGraph_shared(Graph* g, VectorXd &colors)
{
        vector<int> indSet;
        int currentColor = 0;
        Graph* gw = g;

        omp_set_num_threads(THREADS);
        
        while( gw->getNumNodes() != 0 ){
        
          // Compute independent set and assign colors
          mis_shared(gw,indSet); 
          
          #pragma omp parallel for 
          for(int i=0; i<indSet.size(); i++)
                colors(indSet[i]) = currentColor;

          // Remove nodes from working graph and clear independent set for next run
          //removeNodes(&gw, &indSet);
          indSet.clear();

        }

        return 0;
}

int mis_shared(Graph* g, vector<int> &I)
{
	int N = g->getNumNodes();
	int rand[N];
	int u=0;
	vector<int> C;
	VectorXd numNeighbors = VectorXd::Zero(N);
        VectorXd startInd     = VectorXd::Zero(N);

        // For each node: find number of neighbors and 
        // start index for node in edge list
	computeNumNeighbors(g,numNeighbors);
        findStartInd_shared(g,startInd);

	// seed RNG by threadnum
	#pragma omp parallel shared(rand,u,numNeighbors,N)
        {
  	  int i;
  	  unsigned int mySeed = omp_get_thread_num();
  
  	  // Each node gets rand integer in [0,N^4)
  	  #pragma omp for
  	  for( i=0; i < N; i++)
  	  	rand[i] = (rand_r(&mySeed) % (int)(pow((double)N,4)-1));
  
  
        // Currently not working
  	//  while( !C.empty() ){
  
  	//  #pragma omp for 
  	//  for( i=0; i<neighbors(u); i++ ){
  	//    if( rand[u] < rand[i] ){
        //        C.erase(C.begin()+u);
        //        I.push(u);
        //        
        //        for( int j = 0; j<neighbors(u); j++ )
        //          C.erase(C.begin()+g->getEdgePoint(startInd(u)+j,2));
        //      }
        //    } //end while C not empty
        } //end parallel region
		
	return 0;
		
}

int computeNumNeighbors(Graph* g, VectorXd &numNeighbors) 
{

	int k = 0;

	for(int i=0; i<g->getNumNodes(); i++){
	  if(g->getEdgePoint(i,1)==k)
	    numNeighbors(i)++;
	  else
	    k++;
	}

	return 0;
}

int findStartInd_shared(Graph* g, VectorXd &startInd)
{

        int m,n;

        #pragma omp parallel for private(m,n)
        for(int i=1; i<g->getNumEdges(); i++){
          m=(int)g->getEdgePoint(i,1);
          n=(int)g->getEdgePoint(i-1,1);

          if( m!=n)
            startInd(m) = i;
        }

        return 0;
}
