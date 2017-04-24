#ifndef SPECTRALBISECTION_H
#define SPECTRALBISECTION_H

#include<Eigen/Dense>
#include"graph.h"

using namespace Eigen;

MatrixXd spectralBisection(Graph * G);

void getBlocks(MatrixXd A, VectorXd ind1, VectorXd in2, MatrixXd & newA11,MatrixXd & newA12,MatrixXd & newA21,MatrixXd & newA22);

void getIndexSets(VectorXd eigvec2,VectorXd & ind1,VectorXd & ind2);
#endif
