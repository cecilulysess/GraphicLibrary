//
//  matrix.h
//  Graphic
//
//  Created by Julian Wu on 1/20/13.
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//

#ifndef __Graphic__matrix__
#define __Graphic__matrix__

#include<vector>
#include<iostream>
#include<exception>
using std::cout;
using std::endl;
using std::vector;

template <class T>
class Vector {
  vector<T> vec;
  int n;
  
public:
  Vector(int n) {
    this->n = n;
    this->vec = vector<T>(n);
    
  }
  
  void assign_value(int i, T v) {
    this->vec.at(i) = v;
  }
  
  Vector& operator=(const Vector& v) {
    if (this != &v) {
      this->vec.clear();
      this->n = v.n;
      for ( int i = 0 ; i < n; ++i ) {
        this->vec.push_back(v.vec[i]);
      }
    }
    return *this;
  }
  
  const Vector<T>& operator-(){
    for(int i = 0; i < n; ++i ) {
      this->vec[i] *= -1;
    }
    return *this;
  }
  
  T& operator*=(const Vector& v) {
    if ( this->n != v.n ) {
      throw "Vector Length not match";
    }
    T res = this->vec[0] * v.vec[0];
    for ( int i = 1 ; i < n ; ++ i) {
      res += this->vec[i] * v.vec[i];
    }
    return res;
  };
  
  const T& operator*(const Vector& v) {
    Vector<T>* res = new Vector<T>(*this);
    return *res *= v;
    
  }
  
  // Cross product
  Vector& operator%=(const Vector& v) {
    if ( this->n != v.n || this->n != 3 ) {
      throw "Illigle cross product";
    }
    T a1 = this->vec[0], a2 = this->vec[1], a3 = this->vec[2];
    T b1 = v.vec[0], b2 = v.vec[1], b3 = v.vec[2];
    this->vec.clear();
    this->vec.push_back(a2 * b3 - a3 * b2);
    this->vec.push_back( - (a1 * b3 - a3 * b1));
    this->vec.push_back( a1 * b2 - a2 * b1 );
    return *this;
  }
  
  const Vector& operator%(const Vector& v) const {
    Vector<T> * res = new Vector<T>(*this);
    *res %= v;
    return *res;
  }
  
  const T& get(int i) {
    return this->vec.at(i);
  }
  
  void show_vector() const {
    for ( int i = 0; i < n; ++i ) {
      cout<<vec[i]<<"\t";
    }
    cout<<endl;
  }
  
  int size(){
    return n;
  }
};

template <class T>
class Matrix {
  vector<T> matrix;
  // m * n matrix
  int m, n;
  
  
public:
  Matrix(int m, int n){
    this->m = m;
    this->n = n;
    this->matrix = vector<T>();
  }
  
  Matrix(int m, int n, T* data) {
    this->m = m;
    this->n = n;
    this->matrix = vector<T>();
    for ( int i = 0 ; i < m; ++i ) {
      for ( int j = 0; j < n; ++j ) {
        this->matrix.push_back( data[i * n + j] );
      }
    }
  }
  
  Matrix& operator=(const Matrix& m) {
    if ( this != &m ) {
      this->matrix.clear();
      this->m = m.m, this->n = m.n;
      for ( int i = 0; i < m.matrix.size(); ++i ) {
        this->matrix.push_back(m.matrix[i]);
      }
    }
    return *this;
  }
  
  Matrix& operator*=(const Matrix& m){
    if ( this-> n != m.m ) {
      throw "Unmatched matrix";
    } else {
      Matrix<T> temp = *this;
      this->matrix.clear();
      this->n = m.n;
      T subsum = 0.0;
      for ( int i = 0 ; i < this->m; ++ i ) {
        for ( int j = 0 ; j < n; ++j ) {
          subsum = 0.0;
          for ( int k = 0; k < temp.n; ++k ) {
            subsum += temp.get(i, k) * m.get(k, j);
          }
          this->matrix.push_back(subsum);
        }
      }
      return *this;
    }
  }
  
  const Matrix& operator*(const Matrix& m){
    Matrix<T>* res = new Matrix<T>(*this);
    *res *= m;
    return *res;
  }
  
  const Matrix& operator/=(const double d) {
    for (int i = 0; i < this->matrix.size(); ++i ){
      this->matrix[i]/= d;
    }
    return *this;
  }
  
  Matrix& inv() {
    if (m != n || m != 3) {
      throw "Only square matrix is inversable";
    } else {
      Vector<T> a = get_row(0), b = get_row(1), c = get_row(2);
      T D = a * (b % c);
      Matrix<T>* res = new Matrix<T>(*this);
      res->set_row(0, b%c);
      res->set_row(1, (-a)%c);
      res->set_row(2, -a%b);
      *res/=D;
      res->transpose();
      return *res;
    }
    
  }
  
  const T& set_row(int i, Vector<T> row) {
    if (row.size() != this->n ) {
      throw "I llegal assign";
    } else {
      for (int j = 0; j < n; ++j) {
        this->matrix.at(i * n + j) = row.get(j);
      }
    }
  }
  
  const T& get(int i, int j) const {
    if ( i < m && j < n && i >= 0 && j >= 0 ) {
      
      return matrix[i * n + j];
    }else {
      throw "Invalid index";
    }
  }
  
  Matrix<T>& transpose(){
    if ( n == m ) {
      // square transpose
      for (int i = 0; i < m; ++i) {
        for (int j = i; j < n; ++j) {
          T tmp = matrix.at(i * n + j);
          matrix.at(i * n + j) = matrix.at(j * n + i);
          matrix.at(j * n + i) = tmp;
        }
      }
    }
//    int newm = n, newn = m;
//    T tmp =
    return *this;
  }
  
  Matrix<T>& elementwise_product(const Matrix<T>& mat) {
    if(mat.m != this->m && mat.n != this->n) {
      throw "Matrix dimension not match";
    }
    Matrix<T> * res = new Matrix<T>(*this);
    for (int i = 0; i < this->matrix.size(); ++i ){
      res->matrix[i] = this->matrix[i] * mat.matrix[i];
    }
    return *res;
  }
  
  void show_matrix(){
    for ( int i = 0; i < m; ++i ) {
      cout<<"\t";
      for ( int j = 0; j < n; ++j ) {
        cout<<get(i, j)<<"\t";
      }
      cout<<endl;
    }
    cout<<endl;
  }
  
  Vector<T>& get_row(int i) {
    Vector<T>* vec = new Vector<T>(n);
    for (int k = 0; k < n; ++k) {
      vec->assign_value(k, get(i, k));
      
    }
//    vec.show_vector();
    return *vec;
  }
};




#endif /* defined(__Graphic__matrix__) */