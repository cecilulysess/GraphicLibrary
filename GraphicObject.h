#ifndef GRAPHIC_OBJECT_H
#define GRAPHIC_OBJECT_H
#include <iostream>
#include <vector>
#include <string>
#include <set>
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::set;

struct Property {
  float x;
  float y; 
  float z;
  float confidence;
  float intensity;
};

struct ElementFace {
  int coordinate;
  int x;
  int y;
  int z;
};

class kPoint {
public:
  kPoint() : x(0.0), y(0.0), z(0.0) { }
  ~kPoint() { }
  kPoint(const kPoint& kp) {
    x = kp.x; y = kp.y; z = kp.z;  
  }; 
  kPoint& operator=(const kPoint& kp) {
    if (this == &kp) return *this;
    x = kp.x; y = kp.y; z = kp.z;
    return *this;
  }
  kPoint& operator+(const kPoint& kp) {
    x += kp.x;
    y += kp.y;
    z += kp.z;
    return *this;
  }
  kPoint& operator-(const kPoint& kp) {
    x -= kp.x; 
    y -= kp.y;
    z -= kp.z;
    return *this;
  }
  kPoint& operator*(const kPoint& kp) {
    float f1 = y*kp.z-z*kp.y;
    float f2 = z*kp.x-x*kp.z;
    float f3 = x*kp.y-y*kp.x;
    x = f1; y = f2; z = f3;
    return *this;
  }
  kPoint& operator/(const float k) {
    x /= k;
    y /= k;
    z /= k;
    return *this;
  }
  float x;
  float y;
  float z;
};

class GraphicObject {
public:
  GraphicObject();
  ~GraphicObject();
  void readFile();
  void calculateVectors();
  void test();
  float* getProperties();
  unsigned int* getFaces();
  float** getNormalization();
  float* getFaceNormalization();
  float* getPointNormalization();
  inline int getPropertiesLength() { return vertex_num*3; }
  inline int getFacesLength() { return element_face_num*3; }
private:
  int vertex_num;
  int element_face_num;
  vector<Property> properties;    
  vector<ElementFace> faces;
  kPoint* results;
  kPoint* optimal_results;
  float* array_properties;
  unsigned int* array_faces;
  float** array_normalization;
  kPoint* face_normal;
  float* array_face_normalization;
  float* array_point_normalization;
};
#endif
