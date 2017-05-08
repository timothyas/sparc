#include<iostream>
#include<fstream>
#include<iomanip>
#include<math.h>
#include"graph.h"
#include"matrixOperations.h"
#include<ctime>
#include<algorithm>
#include<fstream>
#include<vector>
#include<assert.h>

using namespace std;

std::vector<int> CSC_globalMatVec(std::vector<CSC_MATRIX> Mats,std::vector<CSC_MATRIX> X)
{
	assert(Mats.size()==X.size());
	int numMatVecs = Mats.size();
	std::vector<int> b (Mats[0].n,0);
	std::vector<std::vector<int> > temp_results(numMatVecs,std::vector<int> (Mats[0].n,0));
	for (int i = 0; i < Mats.size(); i++)
	{
		temp_results[i] = CSC_singleMatVec(Mats[i],X[i]);
	}

	for (int i = 0; i < Mats[0].n; i++)
	{
		for (int j = 0; j < numMatVecs; j++)
		{
			b[i] += temp_results[j][i];
		}
	}
	return b;
}

std::vector<int> CSC_singleMatVec(CSC_MATRIX A,CSC_MATRIX X)
{
	int count = 0;
	std::vector<int> b (A.n,0);

	for (int j = 0; j < A.pcol.size(); j++)
	{
		if (X.irow[count]==j)
		{
			for (int i = A.pcol[j]; i < A.pcol[j+1];i++)
			{
				b[A.irow[i]] += A.vals[i]*X.vals[count];
			}
			count++;
		}
	}
	return b; 
}

std::vector<CSC_MATRIX> divideVec(CSC_MATRIX x)
{
	int numSB = 1; 
	int blockSize = x.n/(pow(2,numSB));
	int finalBlockSize = blockSize+ x.n%blockSize;
	int remainder = x.n%blockSize;
	std::vector<CSC_MATRIX> X (pow(2,numSB));
	int val, row;

	for (int i = 0; i < x.irow.size(); i++)
	{
		row = x.irow[i];
		val= x.vals[i];
		if (row >= x.n - remainder)
		{
			X[X.size()-1].irow.push_back(blockSize);
			X[X.size()-1].vals.push_back(val);
		}
		else
		{
			X[row/blockSize].irow.push_back(row%blockSize);
			X[row/blockSize].vals.push_back(val);
		}
	}

	for (int i = 0; i < X.size()-1; i++)
	{
		X[i].n = blockSize;
	}
	X[X.size()-1].n = finalBlockSize;

	return X;
}

std::vector<CSC_MATRIX> getSubMatrices(Graph G)
{
	int numSB = 1; 
	std::vector<CSC_MATRIX> Mats (pow(2,numSB));
	G.sortNeighborList();
	int block_size=G.getNumNodes()/(pow(2,numSB));
	std::vector<int> NE;
	int i; 

	for (i = 0; i < Mats.size()-1; i++)
	{
		Mats[i].n = G.getNumNodes();
		Mats[i].pcol.push_back(0);
		for (int j = block_size*i; j < block_size*(i+1);j++)
		{
			NE = G.getNeighbors(j);
			Mats[i].pcol.push_back(NE.size()+Mats[i].pcol.back());
			for (int k = 0; k < NE.size(); k++)
			{
				Mats[i].irow.push_back(NE[k]);
				Mats[i].vals.push_back(1);
			}
		}
	}

	//the last matrix takes all remaining incase 
	// n isn't divisible by 2
	i = Mats.size()-1;
	Mats[i].pcol.push_back(0);
	Mats[i].n = G.getNumNodes();
	for (int j = block_size*i; j < G.getNumNodes();j++) 
	{
		NE = G.getNeighbors(j);
		Mats[i].pcol.push_back(NE.size()+Mats[i].pcol.back());
		for (int k = 0; k < NE.size(); k++)
		{
			Mats[i].irow.push_back(NE[k]);
			Mats[i].vals.push_back(1);
		}
	}

	return Mats;
}

int saveMatrixToFile(CSC_MATRIX A,std::string filename)
{
	ofstream outFile; 
	outFile.open(filename.c_str());

	int counter; 
	int numMatched;
	int row, col; 

	for (int i = 0; i < A.n;i++)
	{
		numMatched= 0;
		for (int j = 0; j < A.n; j++)
		{
			col = i;
			row = j;
			counter = A.pcol[col] + numMatched;
			if (counter < A.pcol[col+1] && A.irow[counter] == row)
			{
				outFile << A.vals[counter] << " ";
				numMatched++;
			}
			else 
				outFile << 0 << " ";
		}
		outFile << endl;
	}

	/*
	for (int i = 0; i < A.n; i++)
	{
		for (int j = 0; j < A.n; j++)
		{
			outFile << getAij(A,i,j) << " ";
		}
		outFile << endl;
	}
	*/
	outFile.close();
	return 0;
}

double getAij(CSC_MATRIX A,int i,int j)
{
	// flip indices if in upper triangular part of matrix
	if (j > i)
	{
		int temp = i; 
		i = j; 
		j = temp;
	}

	for (int n = A.pcol[j]; n < A.pcol[j+1]; n++)
	{
		if (A.irow[n] == i)
		{
			return A.vals[n];
		}
	}
	return 0.0; 
}
int writeTimingToFile(std::vector<double>& timeKeeper, std::string filename)
{
	ofstream outFile; 
	outFile.open(filename.c_str());

        for(unsigned int i=0; i<timeKeeper.size(); i++){
          outFile << timeKeeper[i] << endl;
        }

        outFile.close();
        return 0;
}
