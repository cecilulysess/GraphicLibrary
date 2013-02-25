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
  int p1;
  int p2;
  int p3;
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
  void readFile(const char* path);
  void calculateVectorsStep1();
  void calculateVectorsStep2();
  void calculateVectorsStep3();
  void execute();
  kPoint getNormal(kPoint, kPoint, kPoint);
  float* getVertexPointer() { return array_triangles; }
  float* getNormalPointer() { return array_normalization;}
  int getNormalLength() { return 9*element_face_num; }
  void test();
  void PrintAll();
private:
  int vertex_num;
  int element_face_num;
  vector<Property> properties;    
  vector<ElementFace> faces;
  float* array_normalization;
  float* sum_normalization;
  float* array_triangles;
};
#endif
