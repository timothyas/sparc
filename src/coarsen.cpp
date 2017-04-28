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

int mxm_shared(Graph* g, vector<int> &colors, int numColors, vector<int> &nodeWeights, vector<int> &matchList)
{
        omp_set_num_threads(THREADS);
        
        int v, vi;
        vector<int>::iterator viter;
        vector<int> lonelyNeighbors;
        vector<int> lonelyWeights;
        vector<int> nodeList;
        vector<int> raceList;

        for(int k=0; k<numColors; k++){

          // Fill nodeList with unmatched nodes of this color
          doubleSelect_shared(colors, k, matchList, -1,nodeList); 

          #pragma omp parallel for private(v,vi,viter,lonelyNeighbors,lonelyWeights)
          for(unsigned int u = 0; u<nodeList.size(); u++){

            // Find unmatched neighbors
            for(unsigned int i = 0; i<g->getNeighbors(nodeList[u]).size(); i++){
              if(matchList[g->getNeighbors(nodeList[u])[i]] == -1){
                #pragma omp critical
                {
                  lonelyNeighbors.push_back(g->getNeighbors(nodeList[u])[i]);
                  lonelyWeights.push_back(nodeWeights[g->getNeighbors(nodeList[u])[i]]);
                }
              }
            }

            if( lonelyWeights.size() != 0 ){

              // Find heaviest lonely neighbor, partner up!
              // Note: max_element returns iterator
              viter = max_element(lonelyWeights.begin(), lonelyWeights.end());
              vi = distance(lonelyWeights.begin(),viter);
              v = lonelyNeighbors[vi];

              // They both swiped right ... 
              matchList[nodeList[u]] = v;
              matchList[v] = nodeList[u];

              #pragma omp critical
              { 
                raceList.push_back(nodeList[u]);
              }
            }

            // Clear neighbors for next round
            lonelyNeighbors.clear();
            lonelyWeights.clear();
        
          } //end parallel region

          cout << " -- pre race check -- " << endl;
          for(int i=0; i<raceList.size(); i++){
            cout << "k: " << k << " rl[" << i << "]: " << raceList[i] << endl;
          }

          #pragma omp parallel for
          for( unsigned int u = 0; u<raceList.size(); u++){
            if( matchList[matchList[raceList[u]]] != raceList[u] )
              matchList[raceList[u]]=-1;
          } //end parallel region

          nodeList.clear();
          raceList.clear();

        } // end k->numColors

        return 0;
}


int colorGraph_shared(Graph* g, vector<int> &colors, int &numColors)
{
        vector<int> indSet;
        vector<int> coloredNodes(g->getNumNodes(), 0);
        int currentColor = 0;
        bool moreColors;

        omp_set_num_threads(THREADS);
        
        // Loop while any nodes have not been assigned a color
  	do{
        
          // Compute independent set and assign colors
          mis_shared(g,coloredNodes,indSet); 
          
          #pragma omp parallel for 
          for(unsigned int i=0; i<indSet.size(); i++){
            colors[indSet[i]] = currentColor;
            coloredNodes[indSet[i]] = 1;
          }

                
          moreColors = any_of(coloredNodes.begin(), coloredNodes.end(), [](int ii){return ii==0 ;});
          indSet.clear();
          currentColor++;

        }while(moreColors);

        numColors = currentColor;

        return 0;
}

int mis_shared(Graph* g, vector<int> finalRemoveList,  vector<int> &I)
{
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
    
        // Loop while any of finalRemoveList are not true.
  	while( any_of(finalRemoveList.begin(), finalRemoveList.end(), [](int ii){return ii==0 ;}) ){

          // Make lists for nodes in current set
          fill(removeList.begin(),removeList.end(),0);
          fill(keepList.begin(),keepList.end(),0);

          #pragma omp parallel for  
          for(int u=0; u<g->getNumNodes(); u++){
            for(unsigned int j=0; j<g->getNeighbors(u).size(); j++){

              if( finalRemoveList[u]==0 ){
                if( rand[u] > rand[g->getNeighbors(u)[j]] && finalRemoveList[g->getNeighbors(u)[j]]==0 ){
                  keepList[u]=1;
                }
                else{
                  removeList[u]=1; 
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

              finalRemoveList[u]=1;

              #pragma omp critical
              {
                I.push_back(u);
              }
            }
          }//end parallel region

            
          // Remove neighbors from independent set
          #pragma omp parallel for 
          for(unsigned int u=0; u<I.size(); u++){
            for(unsigned int j=0; j<g->getNeighbors(I[u]).size(); j++)
              finalRemoveList[g->getNeighbors(I[u])[j]]=1;
          }//end parallel region

        }//end while any node not removed

	return 0;
		
}

int doubleSelect_shared(vector<int> &colors, int currentColor, vector<int> &matchedList, int unmatchedValue, vector<int> &nodeList) 
{
        // This is a glorified parallel select 
        int numSelected;
        vector<int> flag(colors.size(), 0);
        vector<int> flagSum(colors.size(),0);

        #pragma omp for 
	for(unsigned int i=0; i<colors.size(); i++){
          if( colors[i]==currentColor && matchedList[i] == unmatchedValue )
            flag[i]=1;
	}

        flagSum=inclusiveScan_shared(flag);
        numSelected=flagSum[colors.size()-1];

        if( nodeList.size() != 0 )
          nodeList.clear(); 
        nodeList.resize(numSelected);

        if( (int)nodeList.size() != numSelected ){
          cout << "Error in findNeighbors_shared: incorrect size of neighbors vector." << endl;
          return 1;
        }

        #pragma omp for
        for(unsigned int i=0; i<colors.size(); i++){
          if( flag[i]==1 ) 
            nodeList[flagSum[i]-1] = i;
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
