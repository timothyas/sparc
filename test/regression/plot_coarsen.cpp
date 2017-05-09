#include<iostream>
#include<iomanip>
#include<math.h>
#include<ctime>
#include<vector>
#include<omp.h>
#include"graph.h"
#include"coarsen.h"
#include "arlsmat.h"
#include "areig.h"
#include "arlssym.h"
#include "spectralBisection.h"
#include"matrixOperations.h"
using namespace std;

int rewriteEdgeList(std::string readfile, std::string writefile, int maxNode);

int plotCoarsen()
{
	cout << "----------------------------------------------------------------------" << endl; 
	cout << "Output for plotting coarsened graphs" << endl; 
	cout << "----------------------------------------------------------------------" << endl; 

        cout << "Setting up truncated facebook data " << endl;
        
        if(rewriteEdgeList("../../data/facebook_jumbled.txt","../../data/facebook_truncated.txt",200)){
          cout << "Error rewriting edgelist, exiting ... " << endl;
          return 1;
        }


        omp_set_num_threads(12);
        vector<vector<double> > timeKeeper(5,vector<double>(0));
	Graph Graph1("../../data/facebook_truncated.txt");
	Graph Graph2;
	Graph Graph3;

        cout << "Writing facebook_jumbled.txt as csc " << endl;
        std::string fname="../../results/multi-coarsen-12-procs/adj_fb.dat";
        saveCSCMatrixToFile(Graph1,fname);

	cout << "Coarsening level 1" << endl; 
	Graph2.coarsenFrom(Graph1,timeKeeper);
        cout << "Writing 1c as csc " << endl;
        fname="../../results/multi-coarsen-12-procs/adj_c1.dat";
        saveCSCMatrixToFile(Graph2,fname);

	cout << "Coarsening level 2" << endl; 
	Graph3.coarsenFrom(Graph2,timeKeeper);
        cout << "Writing 2c as csc " << endl;
        fname="../../results/multi-coarsen-12-procs/adj_c2.dat";
        saveCSCMatrixToFile(Graph3,fname);


	cout << "Spectral Bisection" << endl; 
	std::vector<int> indMap = spectralBisection(Graph3);

	cout << "Uncoarsening"<< endl;

	indMap = Graph3.reorderGraph(indMap);
        cout << "Writing reordered 2c as csc " << endl;
        fname="../../results/multi-coarsen-12-procs/adj_c2_reorder.dat";
        saveCSCMatrixToFile(Graph3,fname);

	indMap = Graph2.reorderGraph(indMap);
        cout << "Writing reordered 1c as csc " << endl;
        fname="../../results/multi-coarsen-12-procs/adj_c1_reorder.dat";
        saveCSCMatrixToFile(Graph2,fname);

	Graph1.reorderGraph(indMap);
        cout << "Writing reordered fb as csc " << endl;
        fname="../../results/multi-coarsen-12-procs/adj_fb_reorder.dat";
        saveCSCMatrixToFile(Graph1,fname);

	return 0;
}

int rewriteEdgeList(std::string readfile, std::string writefile, int maxNode)
{

        int numnodes, numedges,edge1,edge2,row_counter;
        std::vector<std::vector<int> > edgeList;
        cout << "Reading edgelist from: " << readfile << endl;
        ifstream infile;

        infile.open(readfile.c_str());
        if(!infile.good()){
          cout << "Error reading file: " << readfile << ", exiting ..." << endl;
          return 1;
        }


        infile >> numnodes >> numedges;

        edgeList.resize(numedges,vector<int>(2));
        edge1=0;
        edge2=0;
        row_counter=0;

        infile >> edge1 >> edge2;
        
        while(!infile){
        
          if(edge1 < maxNode && edge2<maxNode){
            edgeList[row_counter][0]=edge1;
            edgeList[row_counter][1]=edge2;
          }

          infile >> edge1 >> edge2;
          row_counter++;
        }
        infile.close();

        // Now write out this list
        ofstream outfile;
        outfile.open(writefile.c_str());

        outfile << maxNode << " " << row_counter << endl;

        for(int i=0; i<row_counter; i++){
          outfile << edgeList[i][0] << " " << edgeList[i][1] << endl;
        }

        outfile.close();
        return 0;
}




















