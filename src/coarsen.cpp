/* File containing routines for coarsening a large
 * sparse graph.
 */

#include<stdlib.h>
#include<math.h>
#include<vector>
#include<omp.h>

#include"graph.h"
#include"coarsen.h"


#define THREADS 4

using namespace std;

int colorGraph_shared(Graph* g, vector<int> &colors)
{
        vector<int> indSet;
        vector<bool> coloredNodes(g->getNumNodes(), false);
        int currentColor = 0;

        omp_set_num_threads(THREADS);
        
        // Loop while any nodes have not been assigned a color
  	while( any_of(coloredNodes.begin(), coloredNodes.end(), [](int ii){return !(ii) ;}) ){
        
          // Compute independent set and assign colors
          mis_shared(gw,coloredNodes,indSet); 
          
          #pragma omp parallel for 
          for(int i=0; i<indSet.size(); i++){
            colors[indSet[i]] = currentColor;
            coloredNodes[indSet[i]] = true;
          }
                
          indSet.clear();
          currentColor++;
        }

        return 0;
}

int mis_shared(Graph* g, vector<bool> &finalRemoveList,  vector<int> &I)
{
	int N = g->getNumNodes();
	int rand[N];
	int u=0;
	vector<int> C = g->getNodeList();
        vector<int> neighbors;
        vector<bool> removeList; 
        vector<bool> keepList; 

	// seed RNG by threadnum
  	unsigned int mySeed = omp_get_thread_num();
  
  	// Each node gets rand integer in [0,N^4)
  	#pragma omp parallel for 
  	for(int i=0; i < N; i++)
 	  rand[i] = (rand_r(&mySeed) % (int)(pow((double)N,4)-1));
  
  
        // Loop while any of finalRemoveList are not true.
  	while( any_of(finalRemoveList.begin(), finalRemoveList.end(), [](int ii){return !(ii) ;}) ){

        // Make lists for nodes in current set
        // Note: std::fill is apparently faster...?
        removeList.assign(C.size,false);
        keepList.assign(C.size,false);

          #pragma omp parallel 
          {
            #pragma omp for private(u,neighbors)
            for(int u=0; u<C.size(); u++){

              findNeighbors_shared(g,u,neighbors);

              //check if node is lonely
              if(neighbors.size()==0)
                removeList[u]=true;
              
              #pragma omp for
              for(int j=0; j<neighbors.size(); j++){

                if( rand[u] > rand[neighbors[j]] )
                  keepList[u]=true;
                else
                  removeList[u]=true; 
              }
            }
          }// end parallel region

          //If both lists agree, add node then flag it and its neighbors
          #pragma omp parallel 
          {
            #pragma omp for private(u,neighbors)
            for(int u=0; u<C.size(); u++){

              if( removeList[u] && !keepList[u] ){
                
                I.push_back(C[u]);
                finalRemoveList[u]=true;
                findNeighbors_shared(g,u,neighbors);

                #pragma omp for
                for(int j=0; j<neighbors.size(); j++)
                  finalRemoveList[neighbors[j]]=true;
              }
            }
          }// end parallel region
        }//end while C not empty
		
	return 0;
		
}

int findNeighbors_shared(Graph* g, vector<bool> &removedNodes, int u, vector<int> &neighbors) 
{

        // This is a glorified parallel select 
        int M = g->getNumEdges();
        int numNeighbors;
        vector<int> flag(M, 0);
        vector<int> flagSum(M,0);

        #pragma omp parallel for 
	for(int i=0; i<M; i++){
          if( !removedNodes[i] && (g->getEdgePoint(i,0) == u || g->getEdgePoint(i,1) == u) )
            flag[i]=1;
	}

        inclusiveScan_shared(flag, flagSum);
        numNeighors=flagSum(n-1);

        // This may or may not be necessary
        neighbors.clear(); 
        neighbors.assign(numNeighbors, 0);

        if( neighbors.size() != numNeighbors ){
          cout << "Error in findNeighbors_shared: incorrect size of neighbors vector." << endl;
          return 1;
        }

        #pragma omp parallel for
        for(int i=0; i<M; i++){
          if( flag[i]==1 && g->getEdgePoint(i,0) == u ) 
            neighbors[flagSum[i]-1] = g->getEdgePoint(i,1);
          else if( flag[i]==1 && g->getEdgePoint(i,1) == u ) 
            neighbors[flagSum[i]-1] = g->getEdgePoint(i,0);
        }
        

	return 0;
}

vector<int> inclusiveScan_shared(vector<int> a)
{
        vector<int> s(a.size(), 0);
        vector<int> b(a.size()/2, 0);
        vector<int> c(a.size()/2, 0);
        
        if( a.size() == 1 ){
          s[0] = a[0];
          return s;
        }

        #pragma omp parallel for
        for( int i=0; i<a.size()/2; i++ )
          b[i] = a[2*i] + a[2*i+1]
         
        c = inclusiveScan_shared( b );

        s[0] = a[0];
        #pragma omp parallel for
        for( int i=1; i<a.size(); i++ ){
          if( i%2 != 0 )
            s[i] = c[i/2];
          else
            s[i] = c[i/2-1] + a[i];
        }

        return s;
}
