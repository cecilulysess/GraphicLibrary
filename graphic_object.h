#ifndef GRAPHIC_OBJECT_H
#define GRAPHIC_OBJECT_H
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include "GraphicMath.h"
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

class GraphicObject {
public:
  GraphicObject();
  ~GraphicObject();
  void readFile();
  void calculateVectors();
  void test();
  inline vector<Property> getProperties() { return properties; }
  inline vector<ElementFace> getFaces() { return faces; }
private:
  int vertex_num;
  int element_face_num;
  vector<Property> properties;    
  vector<ElementFace> faces;
  Vec3d* results;
};

#endif
