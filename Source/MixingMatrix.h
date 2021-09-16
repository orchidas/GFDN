/*
  ==============================================================================

    MixingMatrix.h
    Created: 8 Jul 2021 9:35:37pm
    Author:  Orchisama Das

  ==============================================================================
*/

#pragma once
#include <math.h>
#include <Eigen/Dense>

class MixingMatrix{
    
public:
    MixingMatrix();
    ~MixingMatrix();
    
    void initialize(int nDelLines);      //initialize
    void updateMixingMatrix(float frac);    //update mixing matrix when user changes angle
    Eigen::MatrixXf kroneckerProduct(Eigen::MatrixXf m1, Eigen::MatrixXf m2);    //kronecker product of two matrices
    Eigen::MatrixXf getMixingMatrix();     //access mixing matrix from another class
    bool isOrthonormal(Eigen::MatrixXf X);    //check if matrix is orthonormal
    
    
private:
    Eigen::MatrixXf M;                          //mixing matrix M
    Eigen::Matrix2f rotation2D;                 //2D rotation matrix used to construct M
    float mixingAngle;                          //mixing angle
    int nSize;                                  //size of mixing matrix = ncols = nrows
    const float PI = std::acos(-1);             //pi
    int Nkron;                                  //number of kronecker products needed to form mixing matrix

};


