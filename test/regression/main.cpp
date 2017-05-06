#include<iostream>
#include<omp.h>

using namespace std;

int simple_graph();
int runCoarsenTest();
int complex_graph();
int corner_cases_graph();
int facebook();
int ompTest();  

int main()
{
	omp_set_num_threads(1);

        if(!simple_graph())
		cout << "\033[0;32mPASSED: Simple Graph Test\033[0m"  << endl << endl;
	else
		cout << "\033[0;31mFAILED: Simple Graph Test\033[0m" << endl << endl;

	
	if(!complex_graph())
		cout << "\033[0;32mPASSED: Complex Graph Test\033[0m"  << endl << endl;
	else
		cout << "\033[0;31mFAILED: Complex Graph Test\033[0m" << endl << endl;

	if(!corner_cases_graph())
		cout << "\033[0;32mPASSED: Corner Cases Graph Test\033[0m"  << endl << endl;
	else
		cout << "\033[0;31mFAILED: Corner Cases Graph Test\033[0m" << endl << endl;


        if(!runCoarsenTest())
		cout << "\033[0;32mPASSED: Coarsening Test\033[0m"  << endl << endl;
	else
		cout << "\033[0;31mFAILED: Coarsening Test\033[0m" << endl << endl;

//        if(!facebook())
//		cout << "\033[0;32mPASSED: Facebook Test\033[0m"  << endl << endl;
//	else
//		cout << "\033[0;31mFAILED: Facebook Test\033[0m" << endl << endl;

        if(!ompTest())
		cout << "\033[0;32mPASSED: OpenMP Test\033[0m"  << endl << endl;
	else
		cout << "\033[0;31mFAILED: OpenMP Test\033[0m" << endl << endl;


        return 0;

}
