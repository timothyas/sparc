/* File containing routines for coarsening a large
 * sparse graph.
 */

#include<stdlib.h>
#include<math.h>
#include<vector>
#include<omp.h>
#include<iostream>
#include<algorithm>
#include<time.h>

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
        bool moreColors;

        omp_set_num_threads(THREADS);
        
        // Loop while any nodes have not been assigned a color
  	do{
        
//          for(int i=0; i<g->getNumNodes(); i++){
//            cout << " cNodes: " << coloredNodes[i] <<endl;
//          }

          // Compute independent set and assign colors
          mis_shared(g,coloredNodes,indSet); 
          
          #pragma omp parallel for 
          for(unsigned int i=0; i<indSet.size(); i++){
            colors[indSet[i]] = currentColor;
            coloredNodes[indSet[i]] = 1;
          }

                
          moreColors = any_of(coloredNodes.begin(), coloredNodes.end(), [](int ii){return ii==0 ;});
//          for(unsigned int i=0; i<indSet.size(); i++){
//            cout << "indSet: " << indSet[i] << " color: " << colors[indSet[i]] << endl;
//          }
//          for(int i=0; i<g->getNumNodes(); i++){
//            cout << " cNodes: " << coloredNodes[i] <<endl;
//          }
//          cout << "mc: " << moreColors << endl;
          indSet.clear();
          currentColor++;

        }while(moreColors);

        return 0;
}

int mis_shared(Graph* g, vector<int> finalRemoveList,  vector<int> &I)
{
        time_t timer;
        unsigned int mySeed;
        vector<int> rand(g->getNumNodes(),0);
        vector<int> removeList(g->getNumNodes(),0); 
        vector<int> keepList(g->getNumNodes(),0); 

  	// Each node gets rand integer in [0,N^4)
  	#pragma omp parallel private(mySeed)
        {
          mySeed=(int)time(NULL)*omp_get_thread_num();
          #pragma omp for 
    	  for(int i=0; i < g->getNumNodes(); i++){
            rand[i] = (rand_r(&mySeed) % (int)(pow((double)g->getNumNodes(),4)-1));
          }
        }
    
//        for(int i=0; i<g->getNumNodes(); i++){ 
//          cout << "node: " << i << " r[i]: " << rand[i] << endl; //" timer: " << timer << endl;
//        }
  

        // Loop while any of finalRemoveList are not true.
  	while( any_of(finalRemoveList.begin(), finalRemoveList.end(), [](int ii){return ii==0 ;}) ){

          // Make lists for nodes in current set
          fill(removeList.begin(),removeList.end(),0);
          fill(keepList.begin(),keepList.end(),0);

//          #pragma omp parallel for collapse(2) 
          for(int u=0; u<g->getNumNodes(); u++){
            for(unsigned int j=0; j<g->getNeighbors(u).size(); j++){

//              cout << "u: " << u << " j: " << g->getNeighbors(u)[j] << endl;

              if( finalRemoveList[u]==0 ){
                if( rand[u] > rand[g->getNeighbors(u)[j]] && finalRemoveList[g->getNeighbors(u)[j]]==0 ){
                  keepList[u]=1;
//                  cout << "tid: " << omp_get_thread_num() << " keepList[u="<<u<<"]: "<< keepList[u] << endl;
                }
                else{
                  removeList[u]=1; 
//                  cout << "tid: " << omp_get_thread_num() << " removeList[u="<<u<<"]: "<< removeList[u] << endl;
                }
              }
              else{
                keepList[u]=1;
              }
            }
          }//end parallel region

          // Make independent set, remove from full list
          #pragma omp parallel for  
          for(int u=0; u<g->getNumNodes(); u++){
            if( (removeList[u]==1 || g->getNeighbors(u).size()==0) && keepList[u]==0 ){
              I.push_back(u);
              finalRemoveList[u]=1;
            }
          }

//          for(int i=0; i<finalRemoveList.size(); i++){
//            cout << "frl[" << i << "]: " << finalRemoveList[i] << endl;
//          }
            
          // Remove neighbors from independent set
 //         #pragma omp parallel for collapse(2)
          for(unsigned int u=0; u<I.size(); u++){
            for(unsigned int j=0; j<g->getNeighbors(I[u]).size(); j++)
              finalRemoveList[g->getNeighbors(I[u])[j]]=1;
          }

//          for(int i=0; i<I.size(); i++)
//            cout << "indset: " << I[i] << endl;
//
//          for(int i=0; i<finalRemoveList.size(); i++){
//            cout << "frl[" << i << "]: " << finalRemoveList[i] << endl;
//          }

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

        if( (int)neighbors.size() != numNeighbors ){
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
        for( unsigned int i=0; i<a.size()/2; i++ )
          b[i] = a[2*i] + a[2*i+1];
         
        c = inclusiveScan_shared( b );

        s[0] = a[0];
        #pragma omp for
        for( unsigned int i=1; i<a.size(); i++ ){
          if( i%2 != 0 )
            s[i] = c[i/2];
          else
            s[i] = c[i/2-1] + a[i];
        }

        return s;
}
