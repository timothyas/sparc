#include<iostream>
#include<iomanip>
#include<math.h>
#include<vector>
#include<ctime>
#include<algorithm>
#include<fstream>
#include"graph.h"
#include"matrixOperations.h"

using namespace std;

int matrixManip()
{
	cout << "----------------------------------------------------------------------" << endl; 
	cout << "Running Matrix Manipulation Test" << endl; 
	cout << "----------------------------------------------------------------------" << endl; 

	Graph G("matrixOperations.dat");
	std::vector<CSC_MATRIX> Mats = getSubMatrices(G);

	int irow_temp[] = {1,3,4,0,2,3};
	int pcol_temp[] = {0,3,6};
	std::vector<int> irow_test (irow_temp, irow_temp + sizeof(irow_temp) / sizeof(int) );	
	std::vector<int> pcol_test (pcol_temp, pcol_temp + sizeof(pcol_temp) / sizeof(int) );	

	for (size_t i = 0; i < Mats[0].irow.size(); i++)
		if (Mats[0].irow[i] != irow_test[i])
		{
			cout << 0 << endl;
			cout << i << endl;
			cout << "Expected: " << irow_test[i] << "\tGot: " << Mats[0].irow[i] << endl;
			cout << "Error computing sub matrices [irow]...exiting" << endl;
			return 1; 
		}

	for (size_t i = 0; i < Mats[0].pcol.size(); i++)
		if (Mats[0].pcol[i]!= pcol_test[i])
		{
			cout << 0 << endl;
			cout << i << endl;
			cout << "Expected: " << pcol_test[i] << "\tGot: " << Mats[0].pcol[i] << endl;
			cout << "Error computing sub matrices [pcol]...exiting" << endl;
			return 1; 
		}
	
	int irowtemp[] = {1,3,0,1,2,4,0,3};
	int pcoltemp[] = {0,2,6,8};
	std::vector<int> irowtest (irowtemp, irowtemp + sizeof(irowtemp) / sizeof(int) );	
	std::vector<int> pcoltest (pcoltemp, pcoltemp + sizeof(pcoltemp) / sizeof(int) );	

	for (size_t i = 0; i < Mats[1].irow.size(); i++)
		if (Mats[1].irow[i] != irowtest[i])
		{
			cout << 1 << endl;
			cout << i << endl;
			cout << "Expected: " << irowtest[i] << "\tGot: " << Mats[1].irow[i] << endl;
			cout << "Error computing sub matrices [irow]...exiting" << endl;
			return 1; 
		}

	for (size_t i = 0; i < Mats[1].pcol.size(); i++)
		if (Mats[1].pcol[i] != pcoltest[i])
		{
			cout << 1 << endl;
			cout << i << endl;
			cout << "Expected: " << pcoltest[i] << "\tGot: " << Mats[1].pcol[i] << endl;
			cout << "Error computing sub matrices [pcol]...exiting" << endl;
			return 1; 
		}	
	cout << "Successfully computed sub matrices in CSC format" << endl;

	CSC_MATRIX x; 
	x.irow.push_back(0);
	x.irow.push_back(1);
	x.irow.push_back(2);
	x.irow.push_back(4);
	x.vals.push_back(1);
	x.vals.push_back(1);
	x.vals.push_back(1);
	x.vals.push_back(1);
	x.pcol.push_back(0);
	x.nnz = 4; 
	x.n = 5; 

	std::vector<CSC_MATRIX> X = divideVec(x);
	int B1irowtemp[] = {0,1};
	int B1valstemp[] = {1,1};
	std::vector<int> B1irowtest(B1irowtemp,B1irowtemp+sizeof(B1irowtemp) / sizeof(int));
	std::vector<int> B1valstest(B1valstemp,B1valstemp+sizeof(B1valstemp) / sizeof(int));

	for (size_t i = 0; i < X[0].irow.size(); i++)
		if (X[0].irow[i] != B1irowtest[i])
		{
			cout << "Expected: " << B1irowtest[i] << "\tGot: " << X[0].irow[i] << endl;
			cout << "Error dividing vector [irow]...exiting" << endl;
			return 1; 
		}	
	
	int B2irowtemp[] = {0,2};
	int B2valstemp[] = {1,1};
	std::vector<int> B2irowtest(B2irowtemp,B2irowtemp+sizeof(B2irowtemp) / sizeof(int));
	std::vector<int> B2valstest(B2valstemp,B2valstemp+sizeof(B2valstemp) / sizeof(int));

	for (size_t i = 0; i < X[1].irow.size(); i++)
		if (X[1].irow[i] != B2irowtest[i])
		{
			cout << "Expected: " << B2irowtest[i] << "\tGot: " << X[1].irow[i] << endl;
			cout << "Error dividing vector [irow]...exiting" << endl;
			return 1; 
		}	
	cout << "Successfully divide vector in CSC format" << endl;

	std::vector<double> b  = CSC_globalMatVec(Mats,X);

	int bExacttemp[] = {2,2,1,4,1};
	std::vector<int> bExact(bExacttemp,bExacttemp+sizeof(bExacttemp) / sizeof(int));

	for (size_t i = 0; i < bExact.size();i++)
	{
		if (bExact[i] != b[i])
		{
			cout << "Error performing Matvec...exiting" << endl;
			return 1; 
		}
	}
	cout << "Successfully performed Matvec" << endl; 

	double ExactSolvetemp[] = {0.56837607,  0.12393162,  0.03988604,  0.15384615,  0.11396011};
	std::vector<double> ExactSolve(ExactSolvetemp,ExactSolvetemp+sizeof(ExactSolvetemp) / sizeof(double));

	std::vector<double> v(5,0);
	v[0]=1;

	std::vector<double> Solve = iterSolver(G,v,0.5);
	for (int i = 0; i < Solve.size(); i++)
	{
		if (fabs(Solve[i]-ExactSolve[i]) > 1e-7)
		{
			cout << "Error in iterative solver...exiting" << endl; 
			return 1; 
		}
	}
	cout << "Successfully solved system with iterative solver" << endl; 

	double ExactSolvetemp1[] = { 0.03988604,0.12393162, 0.11396011,0.15384615, 0.56837607};
	std::vector<double> ExactSolve1(ExactSolvetemp1,ExactSolvetemp1+sizeof(ExactSolvetemp1) / sizeof(double));

	std::vector<int> indMap (5,0);
	indMap[0] = 4; 
	indMap[1] = 1; 
	indMap[2] = 0; 
	indMap[3] = 3; 
	indMap[4] = 2; 

	reorderVec(Solve,indMap);
	for (int i = 0; i < Solve.size(); i++)
	{
		if (fabs(Solve[i]-ExactSolve1[i]) > 1e-7)
		{
			cout << "Error reordering vector...exiting" << endl; 
			return 1; 
		}
	}
	cout << "Successfully reordered vector" << endl; 



	return 0;
}
