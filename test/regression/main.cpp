#include<iostream>

using namespace std;

int simple_graph();
int runCoarsenTest();
int complex_graph();

int main()
{

	
        if(!simple_graph())
		cout << "\033[0;32mPASSED: Simple Graph Test\033[0m"  << endl << endl;
	else
		cout << "\033[0;31mFAILED: Simple Graph Test\033[0m" << endl << endl;

	if(!complex_graph())
		cout << "\033[0;32mPASSED: Complex Graph Test\033[0m"  << endl << endl;
	else
		cout << "\033[0;31mFAILED: Complex Graph Test\033[0m" << endl << endl;
		

        if(!runCoarsenTest())
		cout << "\033[0;32mPASSED: Coarsening Test\033[0m"  << endl << endl;
	else
		cout << "\033[0;31mFAILED: Coarsening Test\033[0m" << endl << endl;

        return 0;

}
