#ifndef MATRIXMANIP_H
#define MATRIXMANIP_H

#include"graph.h"
#include<vector>
#include<string>

std::vector<int> CSC_globalMatVec(std::vector<CSC_MATRIX> Mats,std::vector<CSC_MATRIX> X);
std::vector<CSC_MATRIX> getSubMatrices(Graph G);
double getAij(CSC_MATRIX A,int i,int j);
int saveMatrixToFile(CSC_MATRIX A,std::string filename);
std::vector<int> CSC_MatVec(CSC_MATRIX A,CSC_MATRIX X);
std::vector<CSC_MATRIX> divideVec(CSC_MATRIX X);
std::vector<int> CSC_singleMatVec(CSC_MATRIX A,CSC_MATRIX X);
int writeTimingToFile(std::vector<double>& timeKeeper, std::string filename);
#endif
