/* File containing routines for coarsening a large
 * sparse graph.
 */

#include<stdlib.h>
#include<math.h>
#include<vector>
#include<omp.h>
#include<iostream>

#include"graph.h"
#include"coarsen.h"


#define THREADS 4

using namespace std;

//int mxm_shared(Graph* g, 


int colorGraph_shared(Graph* g, vector<int> &colors)
{
        vector<int> indSet;
        vector<int> coloredNodes(g->getNumNodes(), 0);
        int currentColor = 0;

        omp_set_num_threads(THREADS);
        
        // Loop while any nodes have not been assigned a color
  	while( any_of(coloredNodes.begin(), coloredNodes.end(), [](int ii){return ii==0 ;}) ){
        
          // Compute independent set and assign colors
          mis_shared(g,coloredNodes,indSet); 
          
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

int mis_shared(Graph* g, vector<int> &finalRemoveList,  vector<int> &I)
{
	int N = g->getNumNodes();
        vector<int> rand;
        vector<int> removeList; 
        vector<int> keepList; 
	vector<vector<int>> neighbors = g->getNeighborList();

	// seed RNG by threadnum
  	unsigned int mySeed = omp_get_thread_num();
  
  	// Each node gets rand integer in [0,N^4)
  	#pragma omp parallel for 
  	for(int i=0; i < N; i++)
 	  rand[i] = (rand_r(&mySeed) % (int)(pow((double)N,4)-1));
  
  
        // Loop while any of finalRemoveList are not true.
  	while( any_of(finalRemoveList.begin(), finalRemoveList.end(), [](int ii){return ii==0 ;}) ){

          // Make lists for nodes in current set
          // Note: std::fill is apparently faster...?
          fill(removeList.begin(),removeList.end(),0);
          fill(keepList.begin(),keepList.end(),0);

          #pragma omp parallel for collapse(2)  
          for(int u=0; u<g->getNumNodes(); u++){
            for(int j=0; j<(g->getNeighborList(u)).size(); j++){
            for(int j=0; j<neighbors[u].size(); j++){

              if( rand[u] > rand[neighbors[u][j]] )
                keepList[u]=1;
              else
                removeList[u]=1; 
            }
          }//end parallel region

      //    //If both lists agree, add node then flag it and its neighbors
      //    #pragma omp parallel 
      //    {
      //      #pragma omp for  
      //      for(int u=0; u<g->getNumNodes(); u++){

      //        if( removeList[u]==1 && keepList[u]==0 ){
      //          
      //          I.push_back(u);
      //          finalRemoveList[u]=1;

      //          #pragma omp for
      //          for(int j=0; j<neighbors[u].size(); j++)
      //            finalRemoveList[neighbors[u][j]]=1;
      //        }
      //      }
      //    }//end parallel region
      //  }//end while C not empty

          #pragma omp parallel for  
          for(int u=0; u<g->getNumNodes(); u++){
            if( (removeList[u]==1 || neighbors[u].size()==0) && keepList[u]==0 ){
              I.push_back(u);
              finalRemoveList[u]=1;
            }
          }
            
          #pragma omp parallel for collapse(2)
          for(int u=0; u<I.size(); u++){
            for(int j=0; j<neighbors[I[u]].size(); j++)
              finalRemoveList[neighbors[I[u]][j]]=1;
          }

        }//end while C not empty
		
	return 0;
		
}

int findNeighbors_shared(Graph* g, vector<int> &removedNodes, int u, vector<int> &neighbors) 
{
        // Note: this subroutine is called inside a parallel region

        // This is a glorified parallel select 
        int M = g->getNumEdges();
        int numNeighbors;
        vector<int> flag(M, 0);
        vector<int> flagSum(M,0);

        #pragma omp for 
	for(int i=0; i<M; i++){
          if( removedNodes[i]==0 && (g->getEdgePoint(i,0) == u || g->getEdgePoint(i,1) == u) )
            flag[i]=1;
	}

        flagSum=inclusiveScan_shared(flag);
        numNeighbors=flagSum[M-1];

        // This may or may not be necessary
        neighbors.clear(); 
        neighbors.assign(numNeighbors, 0);

        if( neighbors.size() != numNeighbors ){
          cout << "Error in findNeighbors_shared: incorrect size of neighbors vector." << endl;
          return 1;
        }

        #pragma omp for
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

        #pragma omp for
        for( int i=0; i<a.size()/2; i++ )
          b[i] = a[2*i] + a[2*i+1];
         
        c = inclusiveScan_shared( b );

        s[0] = a[0];
        #pragma omp for
        for( int i=1; i<a.size(); i++ ){
          if( i%2 != 0 )
            s[i] = c[i/2];
          else
            s[i] = c[i/2-1] + a[i];
        }

        return s;
}
