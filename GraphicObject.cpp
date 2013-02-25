#include <fstream>
#include <cstdio>
#include <cmath>
#include "GraphicObject.h"
#include <stdio.h>
using std::ifstream;
using std::cout;
using std::endl;

GraphicObject::GraphicObject() : vertex_num(0), 
                                 element_face_num(0),
                                 properties(), 
                                 faces(),
                                 array_normalization(),
                                 sum_normalization(),
                                 array_triangles()
{ }

GraphicObject::~GraphicObject() {
  properties.clear();
  faces.clear();
  delete array_normalization;
  delete sum_normalization;
  delete array_triangles;
}

void GraphicObject::readFile() {
  ifstream myFile;
  myFile.open("test.ply");
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
    myFile >> tf.coordinate >> tf.p1 >> tf.p2 >> tf.p3;
    faces.push_back(tf);
  }
  myFile.close();
}

kPoint GraphicObject::getNormal(kPoint p1, kPoint p2, kPoint p3)
{
   kPoint v1, v2, vp;
   v1.x = p1.x-p2.x;
   v1.y = p1.y-p2.y;
   v1.z = p1.z-p2.z;
   v2.x = p1.x-p3.x;
   v2.y = p1.y-p3.y;
   v2.z = p1.z-p3.z;

   vp.x = v1.y*v2.z - v1.z*v2.y;
   vp.y = v1.z*v2.x - v1.x*v2.z;
   vp.z = v1.x*v2.y - v1.y*v2.x;

   float denominator = sqrt(vp.x*vp.x+vp.y*vp.y+vp.z*vp.z);
   vp.x /= denominator;
   vp.y /= denominator;
   vp.z /= denominator;

   return vp;
}
 

void GraphicObject::calculateVectorsStep1() {
  // create normalization array
  array_normalization = new float[9*element_face_num];
  array_triangles = new float[9*element_face_num];
  kPoint p1, p2, p3, vp;
  for (int i=0; i<element_face_num; i++) {
    array_triangles[9*i]   = properties[faces[i].p1].x;
    array_triangles[9*i+1] = properties[faces[i].p1].y;
    array_triangles[9*i+2] = properties[faces[i].p1].z;
    array_triangles[9*i+3] = properties[faces[i].p2].x;
    array_triangles[9*i+4] = properties[faces[i].p2].y;
    array_triangles[9*i+5] = properties[faces[i].p2].z;
    array_triangles[9*i+6] = properties[faces[i].p3].x;
    array_triangles[9*i+7] = properties[faces[i].p3].y;
    array_triangles[9*i+8] = properties[faces[i].p3].z;
    
    p1.x = properties[faces[i].p1].x;
    p1.y = properties[faces[i].p1].y;
    p1.z = properties[faces[i].p1].z;
    p2.x = properties[faces[i].p2].x;
    p2.y = properties[faces[i].p2].y;
    p2.z = properties[faces[i].p2].z;
    p3.x = properties[faces[i].p3].x;
    p3.y = properties[faces[i].p3].y;
    p3.z = properties[faces[i].p3].z;
    vp = getNormal(p1, p2, p3);  
    array_normalization[9*i] = vp.x;
    array_normalization[9*i+1] = vp.y;
    array_normalization[9*i+2] = vp.z;
    array_normalization[9*i+3] = vp.x;
    array_normalization[9*i+4] = vp.y;
    array_normalization[9*i+5] = vp.z;
    array_normalization[9*i+6] = vp.x;
    array_normalization[9*i+7] = vp.y;
    array_normalization[9*i+8] = vp.z;
  }
}

void GraphicObject::calculateVectorsStep2() {
  sum_normalization = new float[3*vertex_num];
  for (int i=0; i<3*vertex_num; i++) sum_normalization[i] = 0.0;
  int idx_p1, idx_p2, idx_p3;
  for (int i=0; i<element_face_num; i++) {
    idx_p1 = faces[i].p1; idx_p2 = faces[i].p2; idx_p3 = faces[i].p3;
    sum_normalization[3*idx_p1]   += array_normalization[9*i];
    sum_normalization[3*idx_p1+1] += array_normalization[9*i+1];
    sum_normalization[3*idx_p1+2] += array_normalization[9*i+2];
    sum_normalization[3*idx_p2]   += array_normalization[9*i+3];
    sum_normalization[3*idx_p2+1] += array_normalization[9*i+4];
    sum_normalization[3*idx_p2+2] += array_normalization[9*i+5];
    sum_normalization[3*idx_p3]   += array_normalization[9*i+6];
    sum_normalization[3*idx_p3+1] += array_normalization[9*i+7];
    sum_normalization[3*idx_p3+2] += array_normalization[9*i+8];
  }
  for (int i=0; i<3*vertex_num;) {
    float denominator = sqrt(sum_normalization[i]  * sum_normalization[i]
                          + sum_normalization[i+1] * sum_normalization[i+1]
                          + sum_normalization[i+2] * sum_normalization[i+2]);
    sum_normalization[i] /= denominator;
    sum_normalization[i+1] /= denominator;
    sum_normalization[i+2] /= denominator;
    i += 3;  
  }
}

void GraphicObject::calculateVectorsStep3() {
  int idx_p1, idx_p2, idx_p3;
  for (int i=0; i<element_face_num; i++) {
    idx_p1 = faces[i].p1; idx_p2 = faces[i].p2; idx_p3 = faces[i].p3;
    array_normalization[9*i] = sum_normalization[3*idx_p1];
    array_normalization[9*i+1] = sum_normalization[3*idx_p1+1];
    array_normalization[9*i+2] = sum_normalization[3*idx_p1+2];
    array_normalization[9*i+3] = sum_normalization[3*idx_p2];
    array_normalization[9*i+4] = sum_normalization[3*idx_p2+1];
    array_normalization[9*i+5] = sum_normalization[3*idx_p2+2];
    array_normalization[9*i+6] = sum_normalization[3*idx_p3];
    array_normalization[9*i+7] = sum_normalization[3*idx_p3+1];
    array_normalization[9*i+8] = sum_normalization[3*idx_p3+2];
  } 
  /*for (int i=0; i<9*element_face_num; i++) 
    cout << array_triangles[i] << endl;
  cout << "Final Scan" << endl;*/
}

void GraphicObject::execute() {
  readFile();
  calculateVectorsStep1();
  calculateVectorsStep2();
  calculateVectorsStep3();
}

void GraphicObject::test() {
 
}

void GraphicObject::PrintAll() {
  cout<<"Vertexs:"<<endl;
  for (int i = 0; i < vertex_num; ++i) {
    Property& p = properties[i];
    printf("(%f, %f, %f)\n", p.x, p.y, p.z);
  }
}
