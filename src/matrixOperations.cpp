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
#include<omp.h>

using namespace std;

std::vector<double> iterSolver(Graph G,std::vector<double> v, double alpha)
{
	double residual=1; 
	double tol = 1e-14;
	int iterations = 0; 

	std::vector<CSC_MATRIX> Mats = getSubMatrices(G);
	std::vector<CSC_MATRIX> X; 
	CSC_MATRIX D = getDegreeMatrix(G);
	std::vector<double> bn; 
	CSC_MATRIX x = convertVecToCSC(v);
	std::vector<double> bn1 = v; 
	applyScalar(v,1-alpha);
	
	while(residual>tol)
	{
		iterations++; 
		bn = bn1; 
		x = convertVecToCSC(bn);
		applyDinv(D,x);
		X = divideVec(x);
		bn1 = CSC_globalMatVec(Mats,X);
		applyScalar(bn1,alpha);
		addVector(bn1,v);
		residual = computeResidual(bn1,bn);
		if (std::isnan(residual))
		{
			cout << "Error: Iterative solver is diverging...exiting" << endl; 
			assert(std::isnan(residual)==false);
		}
	}

	cout << "--->Converged in " << iterations << " iterations. Residual = " << residual << endl;
	return bn1;
}

int reorderVec(std::vector<double> &b, std::vector<int> indMap)
{
	std::vector<double> oldb = b; 
	if (indMap.size() != b.size())
	{
		cerr << "Error: inconsistent information, cannot reorder" << endl;
		assert(indMap.size() == b.size());
		return 1; 
	}

	for (unsigned int i = 0; i < indMap.size(); i++)
	{
		b[indMap[i]] = oldb[i];
	}

	return 0; 
}

double computeResidual(std::vector<double> bn1,std::vector<double> bn)
{
	double res = 0; 
	if (bn1.size() != bn.size())
	{
		cerr << "Vectors must be of same size to compute residual...exiting" << endl; 
		assert(bn1.size()==bn.size());
		return 1; 
	}

	for (unsigned int i = 0; i < bn.size(); i++)
	{
		res+= pow(bn1[i]-bn[i],2);
	}
	res= pow(res,0.5);
	return res;
}

int addVector(std::vector<double> & b,std::vector<double> v)
{
	if (b.size() != v.size())
	{
		cerr << "Vectors must be of same size to add...exiting" << endl; 
		assert(b.size()==v.size());
		return 1; 
	}

	for (unsigned int i = 0; i < b.size(); i++)
	{
		b[i] += v[i];
	}
	return 0; 

}

int applyScalar(std::vector<double> & v, double Scalar)
{
	for (unsigned int i = 0; i < v.size(); i++)
	{
		v[i]=v[i]*Scalar;
	}
	return 0;
}
	
std::vector<double> CSC_globalMatVec(std::vector<CSC_MATRIX> Mats,std::vector<CSC_MATRIX> X)
{

	assert(Mats.size()==X.size());
	int numMatVecs = Mats.size();
	std::vector<double> b (Mats[0].n,0);
	std::vector<std::vector<double> > temp_results(numMatVecs,std::vector<double> (Mats[0].n,0));

        if( omp_get_num_threads() == 1 ){
	  for (unsigned int i = 0; i < Mats.size(); i++)
	  {
	  	temp_results[i] = CSC_singleMatVec(Mats[i],X[i]);
	  }
        }
        else{

          #pragma omp parallel
          {
  
            if(omp_get_thread_num() < omp_get_num_threads()/2){
              temp_results[0] = CSC_singleMatVec(Mats[0],X[0]);
            }
            else{
              temp_results[1] = CSC_singleMatVec(Mats[1],X[1]);
            }
          }//end parallel region
        }


	/*
	cout << "temp results" << endl;
	for (int i = 0; i < temp_results.size(); i++)
	{
		
		for (int j = 0; j < temp_results[i].size();j++)
		{
			cout << temp_results[i][j] << " ";

		}
		cout << endl;
	}
	*/

	for (int i = 0; i < Mats[0].n; i++)
	{
		for (int j = 0; j < numMatVecs; j++)
		{
			b[i] += temp_results[j][i];
		}
	}
	return b;
}

std::vector<double> CSC_singleMatVec(CSC_MATRIX A,CSC_MATRIX X)
{


	assert(A.pcol.size()-1==X.n); //matrix dimensions agree
	int count = 0;
	std::vector<double> b (A.n,0);

	// if X is all zeros, just return b of all zeros 
	if (X.vals.size() == 0)
	{
		return b; 
	}

	//for (unsigned int j = 0; j < A.pcol.size()-1; j++)
	#pragma omp for schedule(static)
        for (int j =0; j < X.irow.size();j++)
        {
	        for (int i = A.pcol[X.irow[j]]; i < A.pcol[X.irow[j]+1];i++)
		{
			b[A.irow[i]] += A.vals[i]*X.vals[j];
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
	int row; 
	double val;

	for (unsigned int i = 0; i < x.irow.size(); i++)
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

	for (unsigned int i = 0; i < X.size()-1; i++)
	{
		X[i].n = blockSize;
	}
	X[X.size()-1].n = finalBlockSize;

	return X;
}

CSC_MATRIX convertVecToCSC(std::vector<double> b)
{
	CSC_MATRIX x; 
	x.n = b.size();
	for (unsigned int i = 0; i < b.size(); i++)
	{
		if(b[i] != 0)
		{
			x.irow.push_back(i);
			x.vals.push_back(b[i]);
		}
	}
	return x;
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
			for (unsigned int k = 0; k < NE.size(); k++)
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
		for (unsigned int k = 0; k < NE.size(); k++)
		{
			Mats[i].irow.push_back(NE[k]);
			Mats[i].vals.push_back(1);
		}
	}

	return Mats;
}

int applyDinv(CSC_MATRIX D, CSC_MATRIX&  x)
{
	for (unsigned int i = 0; i < x.irow.size(); i++)
	{
		x.vals[i] = x.vals[i]/D.vals[i];
	}

	return 0; 


}

CSC_MATRIX getDegreeMatrix(Graph G)
{
	CSC_MATRIX D; 
	D.n = G.getNumNodes();
	D.irow.resize (D.n,0);
	D.pcol.resize (D.n+1,0);
	D.vals.resize (D.n,0);
	std::vector<int> NE; 

	for (int i = 0; i < D.n; i++)
	{
		NE = G.getNeighbors(i);
		D.irow[i] = i; 
		D.vals[i] = NE.size();
		D.pcol[i] = i; 
	}
	D.pcol[D.n]=D.n;
	return D; 
}

int saveCSCMatrixToFile(Graph G,std::string filename)
{
	G.sortNeighborList();
	CSC_MATRIX A; 
	std::vector<int> NE; 

	A.pcol.push_back(0);
	for (int i = 0; i < G.getNumNodes(); i++)
	{
		NE = G.getNeighbors(i);
		A.pcol.push_back(NE.size()+A.pcol.back());
		for (int j = 0; j < NE.size(); j++)
		{
			A.irow.push_back(NE[j]);
			A.vals.push_back(1);
		}
	}

	ofstream outFile; 
	outFile.open(filename.c_str(),ios::out);

	outFile << endl;
	for (int i = 0; i < A.irow.size(); i++)
	{
		outFile << A.irow[i] << " ";
	}
	outFile << std::endl;

	for (int i = 0; i < A.vals.size(); i++)
	{
		outFile << A.vals[i] << " "; 
	}
	outFile << std::endl;

	for (int i = 0; i < A.pcol.size(); i++)
	{
		outFile << A.pcol[i] << " "; 
	}
	outFile.close();

	return 0;
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
int writeTimingToFile(std::vector<std::vector<double> >& timeKeeper, std::string filename, int coarsen_levels)
{
	ofstream outFile; 
	outFile.open(filename.c_str());

        for(unsigned int i=0; i<timeKeeper.size(); i++){
          for(int j=0; j<coarsen_levels; j++){
            if( j > (int)timeKeeper[i].size()-1 ){
              outFile << setprecision(5) << 0.0 << " ";
            }   
            else{
              outFile << setprecision(5) << timeKeeper[i][j] << " ";
            }
          }
          outFile << endl;
        }

        outFile.close();
        return 0;
}
