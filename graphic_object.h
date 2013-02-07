#ifndef GRAPHIC_OBJECT_H
#define GRAPHIC_OBJECT_H
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::endl;
using std::vector;
using std::string;

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
  void test();
  inline vector<Property> getProperties() { return properties; }
  inline vector<ElementFace> getFaces() { return faces; }
private:
  vector<Property> properties;    
  vector<ElementFace> faces; 
};

#endif
