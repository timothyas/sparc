#include<iostream>
#include<iomanip>
#include<math.h>
//#include"coarsen.h"
#include"graph.h"
#include"spectralBisection.h"
#include<fstream>

using namespace std;


int main()
{

	Graph G("../data/facebook_combined.txt");
	Graph *ptr = &G; 
	spectralBisection(ptr);
	return 0;
}
