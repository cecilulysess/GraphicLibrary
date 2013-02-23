#include <fstream>
#include <cstdio>
#include <assert.h>
#include <cmath>
#include "graphic_object.h"
using std::ifstream;

GraphicObject::GraphicObject() : vertex_num(0), 
                                 element_face_num(0),
                                 properties(), 
                                 faces(), 
                                 results()
{ }

GraphicObject::~GraphicObject() {
  properties.clear();
  faces.clear();
}

void GraphicObject::readFile() {
  ifstream myFile;
  myFile.open("bunny.ply");
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

void GraphicObject::calculateVectors() {
  //vertices.reserve(vertex_num);
  vector<Vec3d> vertices[vertex_num];
  Vec3d p0, p1, p2, v1, v2, vp; // vp: vertex point
  for (int i=0; i<faces.size(); i++) {
    // First Point
    p0.x = properties[faces[i].x].x();
    p0.y = properties[faces[i].x].y();
    p0.z = properties[faces[i].x].z();
    // Second Point
    p1.x = properties[faces[i].y].x();
    p1.y = properties[faces[i].y].y();
    p1.z = properties[faces[i].y].z();
    // Thrid Point
    p2.x = properties[faces[i].z].x();
    p2.y = properties[faces[i].z].y();
    p2.z = properties[faces[i].z].z();
    // get p0 vector and save p0
    v1 = p1 - p0;
    v2 = p2 - p0;
    vp = v1 * v2;
    vertices[faces[i].x].push_back(vp);
    // get p1 vector and save p1
    v1 = p2 - p1;
    v2 = p0 - p1;
    vp = v1 * v2;
    vertices[faces[i].y].push_back(vp);
    // get p2 vector and save p2
    v1 = p0 - p2;
    v2 = p1 - p2;
    vp = v1 * v2;
    vertices[faces[i].z].push_back(vp);
  }
  // sum the vector and normalize it
  results = new Vec3d[vertex_num];
  float rx, ry, rz;
  vector<Vec3d>::iterator it;
  for (int i=0; i< vertex_num; i++) { 
    for (it = vertices[i].begin(); it != vertices[i].end(); it++) 
      results[i] += *it;
    results[i] = results[i].Normalize();
  }
}

void GraphicObject::test() {
  // check if the content read from the file is correct
  cout << "x in first vertex element: " << properties[0].x << endl;
  int size = faces.size() - 1;
  cout << "z in last face element: " << faces[size].z << endl; 
  calculateVectors();
  //cout << "vertics number: " << vertices.size() << endl;
  for (int i=0; i< vertex_num; i++) 
    cout << "(" << results[i].x << ", " << results[i].y << ", " << results[i].z << ")" << endl;
}
