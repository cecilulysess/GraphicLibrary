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



Vec3d::Vec3d(std::initializer_list<double> il) : Vector_<double>(il){
  if (il.size() != 3) {
    throw "Illigal parameter length";
  }
}

Vec3d::Vec3d(const Vec3d& li) : Vector_<double>(li) {
  if (li.size() != 3) {
    throw "Illigal parameter length";
  }
}

Vec3d::Vec3d(Vector_<double> vec) : Vector_<double>(3){
  this->x() = vec[0];
  this->y() = vec[1];
  this->z() = vec[2];
}

Vec3d::Vec3d() : Vector_<double>(3){
  this->x() = 0.0;
  this->y() = 0.0; 
  this->z() = 0.0;
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

Vec3d& Vec3d::operator+=(const Vec3d& v) {
  for (int i = 0; i < this->n; ++i ) {
    this->vec[i] += v.vec[i];
  }
  return *this;
}

Vec3d& Vec3d::operator-=(const Vec3d& v) {
  return (*this)+= -v;
}

const Vec3d& Vec3d::operator+(const Vec3d& v) const {
  Vec3d* res = new Vec3d(*this);
  *res += v;
  return *res;
}

const Vec3d& Vec3d::operator-(const Vec3d& v) const {
  return -((*this) + v);
}


// Cross product
Vec3d& Vec3d::operator%=(const Vec3d& v) {
  double a1 = this->vec[0], a2 = this->vec[1], a3 = this->vec[2];
  double b1 = v.vec[0], b2 = v.vec[1], b3 = v.vec[2];
  this->vec.clear();
  this->vec.push_back(a2 * b3 - a3 * b2);
  this->vec.push_back( - (a1 * b3 - a3 * b1));
  this->vec.push_back( a1 * b2 - a2 * b1 );
  return *this;
}   
   
const Vec3d& Vec3d::operator%(const Vec3d& v) const {
  Vec3d * res = new Vec3d(*this);
  *res %= v;
  return *res;
}   


Vec3d& Vec3d::Normalize() {
  *this *= (1/Norm());
  return *this;
}

Vec3d& Vec3d::Normalization() const{
  Vec3d* res = new Vec3d(*this);
  *res *= (1/Norm());
  return *res;
}

double Vec3d::Norm() const{
  return sqrtl(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

inline double Vec3d::x() const { return vec[0]; };

inline double Vec3d::y() const { return vec[1]; };

inline double Vec3d::z() const { return vec[2]; };

inline double& Vec3d::x() { return vec[0]; };

inline double& Vec3d::y() { return vec[1]; };

inline double& Vec3d::z() { return vec[2]; };
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

//===================================================

Matrix4d::Matrix4d() : Matrixd(4, 4) {
 
}
Matrix4d::Matrix4d(std::initializer_list<std::initializer_list<double>> il) :
  Matrixd(il)
{
  if (row() != col() && row() != 4) {
    throw "Invalid initializer list";
  }
}

double&  Matrix4d::operator[](const int idx){
  if (idx < 0 || idx > mat.size() ) {
    throw "Invalid idx";
  }
  return this->mat[idx];
}

int Matrix4d::Size() { return 4; }

double* Matrix4d::GetPtr() {
  return &mat[0];
}

Matrix4d& Matrix4d::Transpose() {
  Matrix4d* mat = new Matrix4d();
  for (int i = 0; i < this->Size(); ++i) {
    for (int j = 0; j < this->Size(); ++j) {
      mat->set(i, j, get(j, i));
    }
  }
  return *mat;
}

Matrix4d& Matrix4d::Identity() {
  Matrix4d* mat = new Matrix4d();
  for (int i = 0; i < 4; ++i) {
    mat->set(i, i, 1.0);
  }
  return *mat;
}

Matrix4d& Matrix4d::Zeros() {
  return *(new Matrix4d());
}

Matrix4d& Matrix4d::Translate(double x, double y, double z){
  Matrix4d& mat = Matrix4d::Identity();
  mat.set(0, 3, x);
  mat.set(1, 3, y);
  mat.set(2, 3, z);
  return mat;
}

Matrix4d& Matrix4d::Translate(const Vec3d &t_vector) {
  return Matrix4d::Translate(t_vector.x(), t_vector.y(), t_vector.z());
}

Matrix4d& Matrix4d::Scale(double x, double y, double z) {
  Matrix4d& mat = Matrix4d::Identity();
  mat.set(0, 0, x);
  mat.set(1, 1, y);
  mat.set(2, 2, z);
  return mat;
}

Matrix4d& Matrix4d::Scale(double factor) {
  return Matrix4d::Scale(factor, factor, factor);
}

inline double deg2rad(double deg) { return 3.141592653 / 180.0 * deg; }

Matrix4d& Matrix4d::Rotate(double deg, double x, double y, double z) {
  double c = cos(deg2rad(deg)), s = sin(deg2rad(deg));
  double mi = 1 - c, xx = x * x, xy = x * y, xz = x * z, yy = y * y,
         yz = y * z, zz = z * z;
  Matrix4d& mat = Matrix4d::Identity();
  mat.set(0, 0, xx * mi + c);
  mat.set(0, 1, xy * mi - z * s);
  mat.set(0, 2, xz * mi + y * s);
  mat.set(1, 0, xy * mi + z * s);
  mat.set(1, 1, yy * mi + c);
  mat.set(1, 2, yz * mi - x * s);
  mat.set(2, 0, xz * mi - y * s);
  mat.set(2, 1, yz * mi + x * s);
  mat.set(2, 2, zz * mi + c);
  return mat;
}

Matrix4d& Matrix4d::Rotate(double deg, const Vec3d &axis) {
  return Rotate(deg, axis.x(), axis.y(), axis.z());
}

Matrix4d& Matrix4d::RotateX(double deg) {
  return Rotate(deg, 1, 0, 0);
}

Matrix4d& Matrix4d::RotateY(double deg) {
  return Rotate(deg, 0, 1, 0);
}

Matrix4d& Matrix4d::RotateZ(double deg) {
  return Rotate(deg, 0, 0, 1);
}

