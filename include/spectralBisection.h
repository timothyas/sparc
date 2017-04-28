#ifndef SPECTRALBISECTION_H
#define SPECTRALBISECTION_H

#include"graph.h"
#include<vector>


int spectralBisection(Graph *G);
double getAij(CSC_MATRIX A,int i,int j);
//int getIndexSets(vector<double> Eigvec2, std::vector<int> &ind1; std::vector<int> &ind2);
int saveMatrixToFile(CSC_MATRIX A,std::string filename);
int getIndexSets(std::vector<double> Eigvec2, std::vector<int> &ind1, std::vector<int> &ind2);
CSC_MATRIX getBlocks(CSC_MATRIX A, std::vector<int> ind1, std::vector<int> ind2);
#endif
