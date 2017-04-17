#include<Eigen/Eigenvalues>
#include<iostream>
#include<iomanip>
#include<math.h>
#include<Eigen/Dense>
#include"graph.h"
#include"spectralBisection.h"
#include<fstream>

using namespace std;
using namespace Eigen;


int main()
{
	Graph facebook("../data/facebook_combined.txt");
	Graph * G = &facebook; 
	MatrixXd A = G->computeAdjacencyMatrix();
	
	string filename2 = "Matrix.dat";
	ofstream outFile; 
	outFile.open(filename2.c_str());
	for (int i = 0; i < A.rows(); i++)
	{
		for (int j = 0; j < A.cols(); j++)
		{
			outFile << A(i,j) << " ";
		}
		outFile << "\n";
	}
	outFile.close();


	MatrixXd newA = spectralBisection(G);

	string filename= "newMatrix.dat";
	
	outFile.open(filename.c_str());
	for (int i = 0; i < newA.rows(); i++)
	{
		for (int j = 0; j < newA.cols(); j++)
		{
			outFile << newA(i,j) << " ";
		}
		outFile << "\n";
	}
	outFile.close();

	return 0; 
}
