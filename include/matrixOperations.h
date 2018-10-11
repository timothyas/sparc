/** 
 * \file 
 * \author
 *
 * \brief 
 *
 * TODO: We should add information about the page rank problem here and how it 
 * reduces to a matvec / solve. I forgot how we solved it actually so we should 
 * try to add comments about all this stuff. 
 *
 *
 */


#ifndef MATRIXMANIP_H
#define MATRIXMANIP_H

#include"graph.h"
#include<vector>
#include<string>

/* \brief Returns matrix of submatrices (power of 2) 
 * \param G: Full matrix
 * \return Matrix of submatrices
 */
std::vector<CSC_MATRIX> getSubMatrices(Graph G);

/* 
 * \brief Gets the ij element of a given CSC matrix 
 *
 * \param i: Row of matrix 
 * \param j: Column of matrix
 * \return value of matrix A(i,j)
 */
double getAij(CSC_MATRIX A,int i,int j);


/*
 * \brief Saves matrix in csc format to file 
 *
 * \param A: Matrix in CSC format to save
 * \param filename File to save matrix to.
 * \return Error code. 
 */
int saveMatrixToFile(CSC_MATRIX A,std::string filename);
std::vector<CSC_MATRIX> divideVec(CSC_MATRIX X);
int writeTimingToFile(std::vector<std::vector<double> >& timeKeeper, std::string filename, int coarsen_levels);
std::vector<double> CSC_singleMatVec(CSC_MATRIX A,CSC_MATRIX X);
std::vector<double> CSC_globalMatVec(std::vector<CSC_MATRIX> Mats,std::vector<CSC_MATRIX> X);
CSC_MATRIX convertVecToCSC(std::vector<double> b);
CSC_MATRIX getDegreeMatrix(Graph G);
int applyDinv(CSC_MATRIX D, CSC_MATRIX&  x);
int applyScalar(std::vector<double> & v, double Scalar);
int addVector(std::vector<double> & b,std::vector<double> v);
double computeResidual(std::vector<double> bn1,std::vector<double> bn);
std::vector<double> iterSolver(Graph G,std::vector<double> v, double alpha);
int reorderVec(std::vector<double> &b, std::vector<int> indMap);
int saveCSCMatrixToFile(Graph G,std::string filename);
#endif
