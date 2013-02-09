//
//  GraphicMath.h
//  Graphic
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//
//  Author: cosmobserver@gmail.com (Yanxiang Wu <Julian>)
//
//  This files contains a list of data structures that is very helpful for
//  spatial graphic relationship calculation.
//
//  Created on 2/5/13.
//

#ifndef __Graphic__GraphicMath__
#define __Graphic__GraphicMath__

#include <vector>
#include <iostream>
using std::vector;
using std::endl;
using std::cout;
using std::equal;

// Base class of vectors, don't use this class
template <class T>
class Vector {
public:
  explicit Vector(int n) {
    this->n = n;
    this->vec = vector<T>();
  }
  
  Vector(const Vector<T>& li){
    this->n = li.n;
    for (typename std::vector<T>::const_iterator it = li.vec.begin();
          it != li.vec.end(); ++it) {
      vec.push_back(*it);
    }
  }
  
  Vector(std::initializer_list<T> il) {
    if (il.size() > 0){
      this->n = (int)il.size();
      this->vec = vector<T>();
      for (const T* it = il.begin(); it != il.end(); ++it) {
        vec.push_back(*it);
      }
    } else {
      this->n = 0;              //default
      this->vec = vector<T>(); 
    }
  }
  
  virtual ~Vector(){
  }
  
  typename std::vector<T>::const_iterator begin() const {
    return vec.begin();
  }
  
  typename std::vector<T>::const_iterator end() const {
    return vec.end();
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
  
  // Dot product
  T& operator*=(const Vector<T>& v) {
    if ( this->n != v.n ) {
      throw "Vector Length not match";
    }
    T res = this->vec[0] * v.vec[0];
    for ( int i = 1 ; i < n ; ++ i) {
      res += this->vec[i] * v.vec[i];
    }
    return res;
  };
  
  const T& operator*(const Vector<T>& v) const {
    Vector<T>* res = new Vector<T>(*this);
    return *res *= v;
  }
  
  Vector<T>& operator*=(const T& e) {
    for (int i = 0; i < n; ++i) {
      vec[i] *= e;
    }
    return *this;
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
  
  const Vector<T>& operator%(const Vector<T>& v) const {
    Vector<T> * res = new Vector<T>(*this);
    *res %= v;
    return *res;
  }
  
  // do a element-wise mulitplication with v
  const Vector<T>& ele_mult(const Vector<T>& v) const {
    if ( this-> n != v.n ) {
      throw "Unmatched vector length";
    }
    Vector<T> * res = new Vector<T>(*this);
    for (int i = 0; i < n ; ++i) {
      (*res)[i] *= v[i];
    }
    return *res;
  }
  
  T& operator[](const int idx) {
    if (idx < 0 || idx > vec.size() ) {
      throw "Invalid idx";
    }
    return this->vec[idx];
  }
  
  const T& operator[](const int idx) const {
    if (idx < 0 || idx > vec.size() ) {
      throw "Invalid idx";
    }
    return this->vec[idx];
  }
  
  void ShowVector() const {
    for ( T& e : vec) {
      cout<<e<<"\t";
    }
    cout<<endl;
  }
  
  int size() const{
    return n;
  }
  
  
  
protected:
  vector<T> vec;
  int n;
};

template <class T> bool operator==(const Vector<T>& a, const Vector<T>& b) {
  
  if (a.size() != b.size()) return false;
  typename std::vector<T>::const_iterator ita = a.begin();
  typename std::vector<T>::const_iterator itb = b.begin();
  for ( ; ita != a.end(); ++ita, ++itb){
    // a tricky wrapper
    if ( !(*ita == *itb || *ita - *itb < 1e-9)) {
      return false;
    }
  }
  return true;
}

//======================Vector base==============================
// A 3d vector class with doulbe value as element type.
//  This is a good type to use when you want good accuracy
//  Can be initalized by Vec3d pt = {1, 3, 5};
class Vec3d : public Vector<double> {
public:
  Vec3d(std::initializer_list<double> il);
  Vec3d(const Vec3d& li);
  
  Vec3d& operator-() const;
  friend Vec3d& operator*(const double e, const Vec3d& v);
  friend Vec3d& operator*(const Vec3d& v, const double e);
  
  // Auxillary getter
  double x() const ;
  double& x();
  double y() const ;
  double& y();
  double z() const ;
  double& z();
  
  //return the norm of this vector
  double Norm() const;
  //return the normalized version of this vector
  Vec3d& Normalize();
};


//==============================End of Vector================================
// A matrix which contains doulbe as its element type.
class Matrixd {
public:
  Matrixd(std::initializer_list<std::initializer_list<double>> il);
  Matrixd(const Matrixd& mat);
  Matrixd(int row, int col);
  
  Matrixd& operator=(const Matrixd& mat);
  Matrixd& operator*=(const Matrixd& mat);
  Matrixd& operator*(const Matrixd& mat) const;
  Matrixd& operator*=(const double e);
  Matrixd& operator-() const;
  
  friend Matrixd& operator*(const Matrixd& mat, const double e);
  friend Matrixd& operator*(const double e, const Matrixd& mat);
  
  
  // Get the number of rows in this matrix;
  int row() const;
  // Get the number of columns in this matrix
  int col() const;
  //get a value
  double get(int row, int col) const;
  //set a value
  void set(int row, int col, double val);
  //Debug utility
  void ShowMatrix() const;
  
  //Get an identity matrix
  static Matrixd& Identity(int size);

  //Get an zero matrix
  static Matrixd& Zeros(int row, int col);

protected:
  // Generate nxn square matrix that is all zero
  explicit Matrixd(int n);
  vector<double> mat;
  
private:
  int _row, _col;

};

bool operator==(const Matrixd& a, const Matrixd& b) ;

class Matrix4d : public Matrixd {
public:
  Matrix4d(std::initializer_list<std::initializer_list<double>> il);
  Matrix4d();
  
  int Size();
  
  // get a point to the data field
  const double* GetPtr();
  
  // get a transpose of the matrix
  Matrix4d& Transpose();
  
  
  static Matrix4d& Identity();
  static Matrix4d& Zeros();
  
  // get a translate matrix
  // 1 0 0 x
  // 0 1 0 y
  // 0 0 1 z
  // 0 0 0 1
  static Matrix4d& Translate(double x, double y, double z);
  
  static Matrix4d& Translate(const Vec3d& t_vector);
  
  // get a scale matrix
  static Matrix4d& Scale(double factor);
  
  static Matrix4d& Scale(double x, double y, double z);
  
  // get rotation matrix for rotate deg about (x, y, z) axis
  static Matrix4d& Rotate(double deg, double x, double y, double z);
  
  // get rotation matrix for rotate deg about axis
  static Matrix4d& Rotate(double deg, const Vec3d& axis);
  
  // Auxillary short cut for Rotate
  static Matrix4d& RotateX(double deg);
  
  static Matrix4d& RotateY(double deg);
  
  static Matrix4d& RotateZ(double deg);
};


#endif /* defined(__Graphic__GraphicMath__) */
