#include<iostream>
#include<iomanip>
#include<math.h>
#include"string.h"
#include<omp.h>
#include<boost/program_options.hpp>
#include"graph.h"
#include"spectralBisection.h"
#include<fstream>
#include"matrixOperations.h"
#include<ctime>

using namespace std;
using namespace boost::program_options; 

int GetMatrix(Graph & G,int coarsen_levels,CSC_MATRIX & adj, std::vector<int>& indMap);
// Determine if conflicting options are passed
void conflicting_options(const variables_map& vm, const char* opt1, const char* opt2)
{
	if (vm.count(opt1) && !vm[opt1].defaulted()  && vm.count(opt2) && !vm[opt2].defaulted())
		throw logic_error(string("Conflicting options '") + opt1 + "' and '" + opt2 + "'.");
}

int main(int argc, char * argv[])
{
	//Using boost_program_options to parse options 
	try{
		// First add descriptions
		options_description desc("Allowed Options");
		desc.add_options()
			("help,h","show this mesage and exit")
			("filename,f",value<std::string>()->required(),"(required) file containing initial graph data")
			("coarse_levels,c",value<int>()->default_value(4),"Set coarsen levels")
			("nprocs,p",value<int>()->default_value(4),"Set number of procs");
		
		// Second, store command line information
		variables_map vm;
		store(parse_command_line(argc,argv,desc), vm);

		//Print help in certain situations	
		if (argc==1) {
			cout << "Usage: " << argv[0] << " [OPTIONS] [ARGUMENT]" << endl; 
			cout << desc << endl;
			return 0; 
		}

		if (vm.count("help")) {
			cout << "Usage: " << argv[0] << " [OPTIONS] [ARGUMENT]" << endl;
			cout << desc << endl;
			return 0; 
		}

		notify(vm);

	omp_set_num_threads(vm["nprocs"].as<int>());

	string input_file = vm["filename"].as< std::string >();
	int coarsen_levels = vm["coarse_levels"].as<int>();
	Graph G(input_file);
	CSC_MATRIX adj;
	std::vector<int> indMap;
        if(GetMatrix(G,coarsen_levels,adj,indMap)){
          cout << "Error getting index map, exiting ... " << endl;
          return 1;
        }

        std::string outName("Results.dat");
	saveMatrixToFile(adj,outName);
        cout <<  outName << " written" << endl;

        std::string edgeName("edgeList.txt");
        G.writeEdgeList(edgeName);
        cout << edgeName << " written" << endl;
	}
	//Catch errors and exceptions. 
	catch(exception & e) {
		cerr << "error: " << e.what()<< endl;
		return 1; 
	}

	catch(...){
		cerr << "Exception of unknown type!" << endl;
		return 1; 
	}

	return 0;
}

int GetMatrix(Graph & G,int coarsen_levels,CSC_MATRIX & adj, std::vector<int>& indMap)
{
	std::vector<Graph> coarseGraphs (coarsen_levels);
	//std::vector<int> indMap;
	//double start, durations;

	for (int i = 0; i < coarsen_levels;i++)
	{
		if (i==0)
		{
			cout << "Coarsening level " << i+1 << endl; 
			if(coarseGraphs[i].coarsenFrom(G)){
                          cout << "Error coarsening graph at level " << i+1 << endl;
                          return 1;     
                        }
		}
		else
		{
			cout << "Coarsening level " << i+1 << endl; 
			if(coarseGraphs[i].coarsenFrom(coarseGraphs[i-1])){
                          cout << "Error coarsening graph at level " << i+1 << endl;
                          return 1;     
                        }
		}
	}


//	for (int i = 0; i < coarsen_levels; i++)
//	{
//		cout << "LEVEL " << i << endl;
//		for (int j = 0; j < coarseGraphs[i].getNumEdges();j++)
//		{
//			cout << coarseGraphs[i].getEdgePoint(j,0) << " " << coarseGraphs[i].getEdgePoint(j,1) << endl;
//		}
//		cout << endl;
//	}

	cout << "Spectral Bisection on Coarsest Level" << endl;
	if (coarsen_levels==0)
		indMap = spectralBisection(G);
	else
		indMap = spectralBisection(coarseGraphs[coarsen_levels-1]);


//	for (int i = 0; i < indMap.size(); i++)
//	{
//		cout << indMap[i] << endl;
//	}
//
//
	for (int i = coarseGraphs.size()-1 ; i>=0;i--)
	{
		cout << "Uncoarsening level " << i+1 << endl;
		indMap = coarseGraphs[i].reorderGraph(indMap);
	}
//	
//
//	for (int i = 0; i < indMap.size(); i++)
//	{
//		cout << indMap[i] << endl;
//	}

	cout << "Uncoarsening level " << 0<< endl; 
	indMap = G.reorderGraph(indMap);

//	for (int i = 0; i < G.getNumEdges(); i++)
//	{
//		cout << G.getEdgePoint(i,0) << " " << G.getEdgePoint(i,1) << endl;
//	}
//
//	for (int i = 0; i < indMap.size(); i++)
//	{
//		cout << indMap[i] << endl;
//	}
	
	adj = G.computeAdjacencyMatrix();
	return 0;
}
