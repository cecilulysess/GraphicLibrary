//
//  GraphicMath.cpp
//  Graphic
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//
//  Released under Apache 2.0 listence
//
//  Author: cosmobserver@gmail.com (Yanxiang Wu <Julian>)
//
//  Created on 2/5/13.

#include "GraphicMath.h"

#include <iostream>
#include <vector>
#include <math.h>
using std::cout;
using std::endl;

Vec3d::Vec3d(std::initializer_list<double> il) : Vector<double>(il){
  if (il.size() != 3) {
    throw "Illigal parameter length";
  }
}

Vec3d::Vec3d(Vec3d& li) : Vector<double>(li) {
  if (li.size() != 3) {
    throw "Illigal parameter length";
  }
}

Vec3d& Vec3d::Normalize(){
  Vec3d* res = new Vec3d(*this);
  *res *= (1/Norm());
  return *res;
}

double Vec3d::Norm() const{
  return sqrtl(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

Matrixd::Matrixd(std::initializer_list<std::initializer_list<double>> il){
  int col = 0;
  int row = 0;
  for (std::initializer_list<std::initializer_list<double>>::iterator it =
       il.begin(); it != il.end(); ++it ) {
    if (it->size() != col && col != 0) {
      throw "Matrix Column size not matched";
    } else {
      col = (int)it->size();
    }
    row ++;
  }
  for (std::initializer_list<std::initializer_list<double>>::iterator it =
       il.begin(); it != il.end(); ++it ) {
    for (std::initializer_list<double>::iterator nt = (*it).begin();
         nt != (*it).end(); nt++){
      mat.push_back(*nt);
    }
    _row = row;
    _col = col;
  }
}

inline int Matrixd::row() const { return _row; }
inline int Matrixd::col() const { return _col; }

Matrixd::Matrixd(const Matrixd& mat){
  for (int i = 0; i < mat.row() * mat.col(); ++i) {
    this->mat.push_back(mat.mat[i]);
  }
  this->_row = mat.row();
  this->_col = mat.col();
  
}

void Matrixd::ShowMatrix() const {
  for (int i = 0; i < row(); ++i) {
    cout<<"| ";
    cout.width(6);
    for (int j = 0; j < col(); ++j) {
      if (j != col() - 1) {
        cout.flags(std::ios::left);
        cout.width(6);
        cout.precision(4);
      }
      cout<<mat[i * row() + j];
    }
    cout<<" |"<<endl;
  }
  cout<<endl;
}

Matrixd& Matrixd::operator=(const Matrixd& mat) {
  if (this != &mat) {
    this->_row = mat.row();
    this->_col = mat.col();
    this->mat.clear();
    for (int i = 0; i < row() * col(); ++i) {
      this->mat.push_back(mat.mat[i]);
    }
  }
  return *this;
}
