/* File containing routines for coarsening a large
 * sparse graph.
 */

#include<stdlib.h>
#include<math.h>
#include<vector>
#include<Eigen/Dense>

#define THREADS 4

using namespace std;
using namespace Eigen; 

int colorGraph(Graph* g, VectorXd &colors)
{

        


        return 0;
}

int mis(Graph* g, VectorXd &indSet)
{
	int N = g->getNumNodes();
	vector<int> I;
	vector<int> C;
	int rand[N];
	int u=0;
	VectorXd numNeighbors = VectorXd::Zero(N);
        VectorXd startInd     = VectorXd::Zero(N);

        // For each node: find number of neighbors and 
        // start index for node in edge list
	computeNumNeighbors(g,&numNeighbors);
        findStartInd(g,&startInd);

	// seed RNG by threadnum
	#pragma omp parallel default(none) shared(rand,u,neighbors)
        {
	int i;
	unsigned int mySeed = omp_get_thread_num();

	// Each node gets rand integer in [0,N^4)
	#pragma omp for
	for( i=0; i < N; i++)
		rand[i] = (rand_r(&mySeed) % (pow(N,4)-1));


	while( !C.empty() ){

	  #pragma omp for 
	  for( i=0; i<neighbors(u); i++ ){
	    if( rand[u] < rand[i] ){
              C.erase(C.begin()+u);
              I.push(u);
              
              for( int j = 0; j<neighbors(u); j++ )
                C.erase(C.begin()+g->getEdgePoint(startInd(u)+j,2));
            }
          } //end while C not empty
        } //end for i<N
        } //end parallel region
		
	return 0;
		
}

int computeNumNeighbors(Graph* g, VectorXd &neighbors) 
{

	int k = 0;

	for(int i=0; i<g->getNumNodes(); i++){
	  if(g->getEdgePoint(i,1)==k)
	    neighbors(i)++;
	  else
	    k++;
	}

	return 0;
}

int findStartInd(Graph* g, VectorXd &startInd)
{

        #pragma omp parallel for 
        for(int i=1; i<g->getNumEdges(); i++){

          if( g->getEdgePoint(i,1)!=g->getEdgePoint(i-1,1))
            startInd(g->getEdgePoint(i,1)) = i;
        }

        return 0;
}
