#ifndef SPECTRALBISECTION_H
#define SPECTRALBISECTION_H

#include"graph.h"
#include<vector>


std::vector<int> spectralBisection(Graph G);
double getAij(CSC_MATRIX A,int i,int j);
int saveMatrixToFile(CSC_MATRIX A,std::string filename);
std::vector<int> getIndexMap(std::vector<double> Eigvec2);
#endif
