#include<iostream>
#include<iomanip>
#include<math.h>
#include"string.h"
//#include"coarsen.h"
#include"graph.h"
#include"spectralBisection.h"
#include<fstream>

using namespace std;

void usage()
{
	cout << "usage: ./spiC [-h] filename" << endl << endl;
	cout << "positional arguments:" << endl; 
	cout << "\tfilename\tfile containing graph data. Note, file should have number of nodes and number of edges at the top of the file." << endl; 
	cout << "optional arguments:" << endl;
	cout << "\t-h,--help\tshow this help message and exit." << endl; 
}

int main(int argc, char * argv[])
{
	if (argc == 1 || strcmp(argv[1],"-h")==0 || strcmp(argv[1],"--help")==0)
	{
		usage();
		return 0;
	}

	string input_file = argv[1]; 
	Graph G(input_file);
	Graph *ptr = &G; 
	spectralBisection(ptr);
	return 0;
}
