#include <fstream>
#include <cstdio>
#include "graphic_object.h"
using std::ifstream;

GraphicObject::GraphicObject() : properties(), faces() { }

GraphicObject::~GraphicObject() {
  properties.clear();
  faces.clear();
}

void GraphicObject::readFile() {
  ifstream myFile;
  myFile.open("bunny.ply");
  int vertex_num, element_face_num;
  float test_num;
  string ply, format, ascii, trash; float fl;
  myFile >> ply >> format >> ascii >> fl;
  for (int i=0; i<5; i++) myFile >> trash;
  myFile >> vertex_num;
  for (int i=0; i<17; i++) myFile >> trash;
  myFile >> element_face_num;
  for (int i=0; i<6; i++) myFile >> trash;
  properties.reserve(vertex_num);
  faces.reserve(element_face_num);
  Property tp; // temp property
  for (int i=0; i<vertex_num; i++) {
    myFile >> tp.x >> tp.y >> tp.z >> tp.confidence >> tp.intensity;
    properties.push_back(tp);
  }
  ElementFace tf; // temp face
  for (int i=0; i<element_face_num; i++) {
    myFile >> tf.coordinate >> tf.x >> tf.y >> tf.z;
    faces.push_back(tf);
  }
  myFile.close();
}

void GraphicObject::test() {
  // check if the content read from the file is correct
  cout << "x in first vertex element: " << properties[0].x << endl;
  int size = faces.size() - 1;
  cout << "z in last face element: " << faces[size].z << endl; 
}
