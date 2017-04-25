#include<iostream>

using namespace std;

int simple_graph();
int runCoarsenTest();

int main()
{

        cout << "Entering simple graph test ... " << endl;
        simple_graph();
        cout << "Exiting simple graph test ... " << endl;

        cout << "Entering graph coarsening test ... " << endl;
        runCoarsenTest();
        cout << "Exiting graph coarsening test ... " << endl;

        return 0;

}
