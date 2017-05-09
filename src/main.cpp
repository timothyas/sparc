#include<iostream>
#include<iomanip>
#include<fstream>
#include<math.h>
#include"string.h"
#include<omp.h>
#include<boost/program_options.hpp>
#include"graph.h"
#include"spectralBisection.h"
#include<fstream>
#include"matrixOperations.h"
#include<ctime>
#include<sys/time.h>
#include<exception>
#include<assert.h>
#include<stdexcept>
#include<numeric>

using namespace std;
using namespace boost::program_options; 

int PrepareGraph(Graph & G,int coarsen_levels,std::vector<int>& indMap, std::vector<std::vector<double> >& timeKeeper);
std::vector<double> readV(std::string file);
int saveVec(std::vector<double> b, std::string file);
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
			("graphFile,f",value<std::string>()->required(),"(required) file containing initial graph data")
			("alpha,a",value<double>()->default_value(0.5),"Set probability praramer alpha")
			("coarse_levels,c",value<int>()->default_value(0),"Set coarsen levels")
			("noSB,n",bool_switch(),"Don't do spectral bisection. Just solve pagerank problem with original graph.")
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

	//Get inputs 
	omp_set_num_threads(vm["nprocs"].as<int>());
	string input_file = vm["graphFile"].as< std::string >();
	int coarsen_levels = vm["coarse_levels"].as<int>();
	double alpha = vm["alpha"].as<double>();

	//Set up and solve for page rank vector
	Graph G(input_file);
	cout << "Reading in V" << endl;
	std::vector<double> v = readV("VinFile.dat");
        std::vector<std::vector<double> > timeKeeper(5,vector<double>(0));

	std::vector<int> indMap;
	if (!vm["noSB"].as<bool>())
	{
	        if(PrepareGraph(G,coarsen_levels,indMap,timeKeeper))
		{
       		   cout << "Error getting index map, exiting ... " << endl;
       		   return 1;
		}
		std::string reEdge("ReorderedGraph.txt");
		G.writeEdgeList(reEdge);
		cout << reEdge << " written" << endl;
		cout << "Reordering Input Vector" << endl;
		if(reorderVec(v,indMap))
		{
			return 1; 
		}
	}
	
        struct timeval start, end;
	cout << "Solving Linear System" << endl;
        gettimeofday(&start,NULL);
	std::vector<double> b = iterSolver(G,v,alpha);

        gettimeofday(&end,NULL);
        timeKeeper[4].push_back(((end.tv_sec - start.tv_sec)*1000000u + end.tv_usec - start.tv_usec) / 1.e6);
        cout << setprecision(5) << "--->Linear system solved (Time: " << timeKeeper[4][0] << ")" << endl;

	if (!vm["noSB"].as<bool>())
	{
		cout << "Reordering Answer" << endl;
		if(reorderVec(b,indMap))
	{
		return 1; 
	}
	}
	cout << "Writing Results" << endl;
	//Save Results
	std::string outName = "Results.dat";
	//CSC_MATRIX adj = G.computeAdjacencyMatrix();
	saveCSCMatrixToFile(G,outName);
        cout <<  outName << " written" << endl;

	std::string prvec = "Page_Rank_Vec.txt";
	saveVec(b,prvec);

        std::string edgeName("edgeList.txt");
        G.writeEdgeList(edgeName);
        cout << edgeName << " written" << endl;

        std::string timeFile("timing.txt");
        writeTimingToFile(timeKeeper,timeFile,coarsen_levels);
        cout << timeFile << " written" << endl;

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

int PrepareGraph(Graph & G, int coarsen_levels, std::vector<int>& indMap, std::vector<std::vector<double> >& timeKeeper)
{
	std::vector<Graph> coarseGraphs (coarsen_levels);
        struct timeval start, end;

	for (int i = 0; i < coarsen_levels;i++)
	{
		if (i==0)
		{
			cout << "Coarsening level " << i+1 << endl; 
			if(coarseGraphs[i].coarsenFrom(G,timeKeeper)){
                          cout << "Error coarsening graph at level " << i+1 << endl;
                          return 1;     
                        }
                        cout << setprecision(5) << "--->Level " << i+1 << " complete (Time: ";
                        cout << timeKeeper[0][i]+timeKeeper[1][i]+timeKeeper[2][i] << ")" << endl;
		}
		else
		{
			cout << "Coarsening level " << i+1 << endl; 
			if(coarseGraphs[i].coarsenFrom(coarseGraphs[i-1],timeKeeper)){
                          cout << "Error coarsening graph at level " << i+1 << endl;
                          return 1;     
                        }
                        cout << setprecision(5) << "--->Level " << i+1 << " complete (Time: ";
                        cout << timeKeeper[0][i]+timeKeeper[1][i]+timeKeeper[2][i] << ")" << endl;
		}
	}

	cout << "Spectral Bisection on Coarsest Level" << endl;
	if (coarsen_levels==0){
                gettimeofday(&start,NULL);
		indMap = spectralBisection(G);
                gettimeofday(&end,NULL);
                timeKeeper[3].push_back(((end.tv_sec - start.tv_sec)*1000000u + end.tv_usec - start.tv_usec) / 1.e6);
                cout << setprecision(5) << "--->Spectral Bisection complete (Time: " << timeKeeper[3][0] << ")" << endl;
        }       

	else{
                gettimeofday(&start,NULL);
		indMap = spectralBisection(coarseGraphs[coarsen_levels-1]);
                gettimeofday(&end,NULL);
                timeKeeper[3].push_back(((end.tv_sec - start.tv_sec)*1000000u + end.tv_usec - start.tv_usec) / 1.e6);
                cout << setprecision(5) << "--->Spectral Bisection complete (Time: " << timeKeeper[3][0] << ")" << endl;
        }

	for (int i = coarseGraphs.size()-1 ; i>=0;i--)
	{
		cout << "Uncoarsening level " << i+1 << endl;
		indMap = coarseGraphs[i].reorderGraph(indMap);
	}

	cout << "Uncoarsening level " << 0<< endl; 
	indMap = G.reorderGraph(indMap);

	return 0;
}

std::vector<double> readV(std::string file)
{
	std::vector<double> b; 
	double val; 
	ifstream inFile;
	inFile.open(file.c_str());
	assert(inFile.good()!=0);
	inFile >> val; 
	while(!inFile.eof())
	{
	        b.push_back(val);
		inFile >> val; 
	}
        inFile.close();

	return b;
}

int saveVec(std::vector<double> b, std::string file)
{
	ofstream outFile; 
	outFile.open(file.c_str());
	for (int i = 0; i < b.size();i++)
	{
		outFile << b[i] << "\n";
	}
	outFile.close();
	return 0;

}

