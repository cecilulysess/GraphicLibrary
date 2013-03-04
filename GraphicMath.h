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

// 
// Base class of vectors, don't use this class
 template <class T>
 class Vector_ {
 public:
   explicit Vector_(int n) {
     this->n = n;
     this->vec = vector<T>();
   }
   
   Vector_(const Vector_<T>& li){
     this->n = li.n;
     for (typename std::vector<T>::const_iterator it = li.vec.begin();
           it != li.vec.end(); ++it) {
       vec.push_back(*it);
     }
   }
   
   Vector_(std::initializer_list<T> il) {
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
   
   virtual ~Vector_(){
   }
   
   typename std::vector<T>::const_iterator begin() const {
     return vec.begin();
   }
   
   typename std::vector<T>::const_iterator end() const {
     return vec.end();
   }
   
   Vector_<T>& operator-=(const Vector_<T>& v) {
     if (this->n != v.n) {
       throw "Unmatched vector length for operator-";
     }
     for (int i = 0; i < this->n; ++i ) {
       this->vec[i] -= v.vec[i];
     }
     return *this;
   }
   
   const Vector_<T>& operator-(const Vector_<T>& v) const {
     Vector_<T> * res = new Vector_<T>(*this);
     *res -= v;
     return *res;
   }
   
   Vector_<T>& operator+=(const Vector_<T>& v) {
     if (this->n != v.n) {
       throw "Unmatched vector length for operator-";
     }
     for (int i = 0; i < this->n; ++i ) {
       this->vec[i] += v.vec[i];
     }
     return *this;
   }
   
   const Vector_<T>& operator+(const Vector_<T>& v) const {
     Vector_<T> * res = new Vector_<T>(*this);
     *res += v;
     return *res;
   }
   
   Vector_<T>& operator=(const Vector_<T>& v) {
     if (this != &v) {
       this->vec.clear();
       this->n = v.n;
       for ( int i = 0; i < n; ++i ) {
         this->vec.push_back(v.vec[i]);
       }
     }
     return *this;
   }
   
   // Dot product
   T& operator*=(const Vector_<T>& v) {
     if ( this->n != v.n ) {
       throw "Vector_ Length not match";
     }
     T res = this->vec[0] * v.vec[0];
     for ( int i = 1 ; i < n ; ++ i) {
       res += this->vec[i] * v.vec[i];
     }
     return res;
   };
   
   const T& operator*(const Vector_<T>& v) const {
     Vector_<T>* res = new Vector_<T>(*this);
     return *res *= v;
   }
   
   Vector_<T>& operator*=(const T& e) {
     for (int i = 0; i < n; ++i) {
       vec[i] *= e;
     }
     return *this;
   }
   
   
   // do a element-wise mulitplication with v
   const Vector_<T>& ele_mult(const Vector_<T>& v) const {
     if ( this-> n != v.n ) {
       throw "Unmatched vector length";
     }
     Vector_<T> * res = new Vector_<T>(*this);
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
   
   void ShowVector_() const {
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
 
 template <class T> bool operator==(const Vector_<T>& a, const Vector_<T>& b) {
   
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
 
 //======================Vector_ base==============================
 // A 3d vector class with doulbe value as element type.
 //  This is a good type to use when you want good accuracy
 //  Can be initalized by Vec3d pt = {1, 3, 5};
 // Methods:
 //  Vec3d v = {1, 2, 3} // greate a vector with x == 1, y == 2
 //    z == 3
 //  Vec3d t(v)
 //  t =  -v * 3;
 //  t =  3 * -v;
 //  t = t % v  // cross product 
 //  t %= v     // t = t % v
 //  t *= 3     // t = t * 3
 //  t.Norm()   // get t's norm
 //  t.Normalize() //return a copy of normalized t
 //  t.x(), t.y(), t.z()  // get xyz component
 //  t[0], t[1], t[2] // access xyz component
 class Vec3d : public Vector_<double> {
 public:
   Vec3d(std::initializer_list<double> il);
   Vec3d(const Vec3d& li);
   Vec3d();
   // create a Vec3d from vec
   explicit Vec3d(Vector_<double> vec);
   
   Vec3d& operator-() const;
   friend Vec3d& operator*(const double e, const Vec3d& v);
   friend Vec3d& operator*(const Vec3d& v, const double e);
   Vec3d& operator+=(const Vec3d& v);
   Vec3d& operator-=(const Vec3d& v);
   const Vec3d& operator+(const Vec3d& v) const;
   const Vec3d& operator-(const Vec3d& v) const;
   Vec3d& operator%=(const Vec3d& v);
   const Vec3d& operator%(const Vec3d& v) const;
   
   // Auxillary getter
   double x() const ;
   double& x();
   double y() const ;
   double& y();
   double z() const ;
   double& z();
   
   //return the norm of this vector
   double Norm() const;
   //return the normalized version of this vector, this will change self
   Vec3d& Normalize();
   // return a copy ot normalized version of this vector
   Vec3d& Normalization() const;
 };
 
 
//==============================End of Vector_================================
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
  
  double Determinant() const;
  //get inversion of matrix
  Matrixd inv() const;
  
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
  Matrixd LU_Decompose(const Matrixd& M, int *idx) const;
  void LU_back_substitution(const Matrixd& M, int *indx, double col[]) const;
  
};

bool operator==(const Matrixd& a, const Matrixd& b) ;

class Matrix4d : public Matrixd {
public:
  Matrix4d(std::initializer_list<std::initializer_list<double>> il);
  Matrix4d();
  
  double& operator[](const int idx);
  int Size();
  
  // get a point to the data field
  double* GetPtr();
  
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
