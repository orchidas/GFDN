/*
  ==============================================================================

    MixingMatrix.cpp
    Created: 9 Jul 2021 1:22:14am
    Author:  Orchisama Das

  ==============================================================================
*/

#include "MixingMatrix.h"

MixingMatrix::MixingMatrix(){};
MixingMatrix::~MixingMatrix(){};

//initialize matrix
void MixingMatrix::initialize(int nDelLines){
    nSize = nDelLines;
    Nkron = (int) log2(nSize);
    mixingAngle = 0.0f;
    M.resize(nSize, nSize);
    M.setIdentity();
}

//update matrix when user changes mixing angle
void MixingMatrix::updateMixingMatrix(float frac){
    
    //create 2D rotation matrix
    mixingAngle = frac/(PI/4.0f);   //frac is fraction of mixing from 0 to 1
    rotation2D << std::cos(mixingAngle), std::sin(mixingAngle),
    -std::sin(mixingAngle), std::cos(mixingAngle);
    
    //kronecker with itself a bunch of times
    Eigen::MatrixXf temp = rotation2D;
    Eigen::MatrixXf kronProd;
    for(int i = 0; i < Nkron-1; i++){
        kronProd = kroneckerProduct(rotation2D, temp);
        //temp.resize(kronProd.rows(), kronProd.cols());    //use if there is an error
        temp = kronProd;
        //kronProd.resize(kronProd.rows()*2, kronProd.cols()*2);
    }
    M = temp;
}

//kronecker product of two matrices
Eigen::MatrixXf MixingMatrix::kroneckerProduct(Eigen::MatrixXf m1, Eigen::MatrixXf m2)
{
    Eigen::MatrixXf m3(m1.rows()*m2.rows(), m1.cols()*m2.cols());
    
    for (int i = 0; i < m1.cols(); i++) {
        for (int j = 0; j < m1.rows(); j++) {
            m3.block(i*m2.rows(), j*m2.cols(), m2.rows(), m2.cols()) =  m1(i,j)*m2;
        }
    }
    return m3;
}

//return mixing matrix
Eigen::MatrixXf MixingMatrix::getMixingMatrix(){
    return M;
}

//helper function to ensure matrix is orthonormal
bool MixingMatrix::isOrthonormal(Eigen::MatrixXf X){
    Eigen::MatrixXf innerProd = X*X.transpose();
    if (innerProd.isApprox(Eigen::MatrixXf::Identity(X.rows(), X.cols())))
        return true;
    else
        return false;
}

