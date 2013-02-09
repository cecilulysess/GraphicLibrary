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

Vec3d::Vec3d(const Vec3d& li) : Vector<double>(li) {
  if (li.size() != 3) {
    throw "Illigal parameter length";
  }
}

Vec3d& Vec3d::operator-() const {
  return (*this * -1.0);
}

Vec3d& operator*(const Vec3d& v, const double e) {
  return e * v;
}

Vec3d& operator*(const double e, const Vec3d& v) {
  Vec3d* res = new Vec3d(v);
  *res *= e;
  return *res;
}

Vec3d& Vec3d::Normalize(){
  Vec3d* res = new Vec3d(*this);
  *res *= (1/Norm());
  return *res;
}

double Vec3d::Norm() const{
  return sqrtl(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}


//============================Matrix======================================
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
  }
  row = il.size();
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

Matrixd::Matrixd(int n){
  this->mat = vector<double>(n*n);
  _row = n;
  _col = n;
}

Matrixd::Matrixd(int row, int col){
  this->mat = vector<double>(row * col);
  _row = row;
  _col = col;
}

inline double Matrixd::get(int row, int col) const {
  if (row < 0 || row > _row || col < 0 || col > _col) {
    throw "Specified row and col index out of boundary";
  }
  return this->mat[_col * row + col];
}

inline void Matrixd::set(int row, int col, double val) {
  if (row < 0 || row > _row || col < 0 || col > _col) {
    throw "Specified row and col index out of boundary";
  }
  mat[row * _col + col] = val;
  
}

void Matrixd::ShowMatrix() const {
  for (int i = 0; i < row(); ++i) {
    cout<<"| ";
    cout.width(6);
    for (int j = 0; j < col(); ++j) {
      if (j != col() - 1) {
        cout.flags(std::ios::left);
        cout.width(9);
        cout.precision(6);
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


Matrixd& Matrixd::operator*=(const Matrixd& mat) {
  if (this->col() != mat.row() ) {
    throw "Matrix Dimension Not Matched";
  }
  Matrixd tmp(this->row(), mat.col());
  for (int i = 0; i < this->row(); ++i) {
    for (int j = 0; j < mat.col(); ++j) {
      long double elesum = 0.0;
      for (int k = 0; k < this->col(); ++k) {
        elesum += this->get(i, k) * mat.get(k, j);
      }
      tmp.set(i, j, elesum);
    }
  }
  (*this) =  tmp;
  return *this;
}

Matrixd& Matrixd::operator*=(const double e){
  for (int i = 0; i < this->row() * col(); ++i ) {
    this->mat[i] *= e;
  }
  return *this;
}

Matrixd& Matrixd::operator*(const Matrixd& mat) const{
  Matrixd* res = new Matrixd(*this);
  *res *= mat;
  return *res;
}

Matrixd& operator*(const Matrixd& mat, const double e){
  Matrixd* res = new Matrixd(mat);
  *res *= e;
  return *res;
}

Matrixd& operator*(const double e, const Matrixd& mat) {
  return mat * e;
}

Matrixd& Matrixd::operator-() const{
  return *this * -1.0;
}

bool operator==(const Matrixd& a, const Matrixd& b) {
  if (a.row() != b.row() && a.col() != b.col()) {
    return false;
  }
  for (int i = 0; i < a.row(); ++i ) {
    for (int j = 0; j < a.col(); ++j ) {
      if ( fabs(a.get(i, j) - b.get(i, j)) > fabs(a.get(i, j)/1e6)) {
        if (fabs(a.get(i, j) - b.get(i, j)) < 1) {
          cout<<"May be unreliable double calculation result in unequal"<<
          " check the value"<<endl;
        }
        cout<<"Unequal find at:"<<i<<", "<<
          j<<" a:"<<a.get(i, j)<<" b:"<<b.get(i,j)<<" diff:"<<
          fabs(a.get(i, j) - b.get(i, j))<<" test:" <<
          fabs(a.get(i, j)/1e9)<<endl;
        return false;
      }
    }
  }
  return true;
}

Matrixd& Matrixd::Identity(int size){
  Matrixd* mat = new Matrixd(size);
  for (int i = 0; i < size; ++i) {
    mat->set(i, i, 1.0);
  }
  return *mat;
}

Matrixd& Matrixd::Zeros(int row, int col) {
  Matrixd* mat = new Matrixd(row, col);
  return * mat;
}
