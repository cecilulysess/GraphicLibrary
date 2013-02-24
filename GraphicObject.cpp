#include <fstream>
#include <cstdio>
#include <cmath>
#include "GraphicObject.h"
using std::ifstream;

GraphicObject::GraphicObject() : vertex_num(0), 
                                 element_face_num(0),
                                 properties(), 
                                 faces(), 
                                 results(),
                                 optimal_results(),
                                 array_properties(),
                                 array_faces(),
                                 array_normalization(),
                                 face_normal(),
                                 array_face_normalization(),
                                 array_point_normalization()
{ }

GraphicObject::~GraphicObject() {
  properties.clear();
  faces.clear();
  //delete results;
  //delete optimal_results;
  delete array_properties;
  delete array_faces;
  for (int i=0; i<vertex_num; i++) 
    delete array_normalization[i];
  delete array_normalization;
  delete array_face_normalization;
  delete array_point_normalization;
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
  vector<kPoint> vertices[vertex_num];
  kPoint p0, p1, p2, v1, v2, vp; // vp: vertex point
  face_normal = new kPoint[faces.size()];
  for (int i=0; i<faces.size(); i++) {
    // First Point
    p0.x = properties[faces[i].x].x;
    p0.y = properties[faces[i].x].y;
    p0.z = properties[faces[i].x].z;
    // Second Point
    p1.x = properties[faces[i].y].x;
    p1.y = properties[faces[i].y].y;
    p1.z = properties[faces[i].y].z;
    // Thrid Point
    p2.x = properties[faces[i].z].x;
    p2.y = properties[faces[i].z].y;
    p2.z = properties[faces[i].z].z;
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
    // get face normal
    face_normal[i].x = vp.x/sqrt(vp.x*vp.x + vp.y*vp.y + vp.z*vp.z);
    face_normal[i].y = vp.y/sqrt(vp.x*vp.x + vp.y*vp.y + vp.z*vp.z);
    face_normal[i].z = vp.z/sqrt(vp.x*vp.x + vp.y*vp.y + vp.z*vp.z);
  }
  // test correctness
  cout << "the vertices[50] size: " << vertices[50].size() << endl;
  // sum the vector and normalize it
  results = new kPoint[vertex_num];
  optimal_results = new kPoint[vertex_num];
  float rx, ry, rz;

  vector<kPoint>::iterator it;
  for (int i=0; i< vertex_num; i++) { 
    for (it = vertices[i].begin(); it != vertices[i].end(); it++) 
      results[i] = results[i] + *it;
    rx = results[i].x; ry = results[i].y; rz = results[i].z;
    results[i] = results[i] / sqrt(rx*rx + ry*ry + rz*rz);
    if (rx == 0 && ry == 0 && rz == 0) {
      optimal_results[i].x = 0; optimal_results[i].y = 0; optimal_results[i].z = 0;  
    } else {
      optimal_results[i] = results[i];  
    }
  }
}

float* GraphicObject::getProperties() {
  //float array[length];
  vector<float> va;
  for (int i=0; i<vertex_num; i++) {
    va.push_back(properties[i].x);
    va.push_back(properties[i].y);
    va.push_back(properties[i].z);
  }
  int length = va.size();
  array_properties = new float[length];
  for (int i=0; i<length; i++) array_properties[i] = va[i];
  return array_properties; 
}

unsigned int* GraphicObject::getFaces() {
  vector<int> vf;
  for (int i=0; i<element_face_num; i++) {
    vf.push_back(faces[i].x);  
    vf.push_back(faces[i].y);  
    vf.push_back(faces[i].z);  
  }
  int length = vf.size(); 
  array_faces = new unsigned int[length];
  for (int i=0; i<length; i++) array_faces[i] = vf[i];
  return array_faces;
}

float* GraphicObject::getFaceNormalization() {
  vector<float> vfn;
  for (int i=0; i<element_face_num; i++) {
    vfn.push_back(face_normal[i].x);  
    vfn.push_back(face_normal[i].y);  
    vfn.push_back(face_normal[i].z);  
  }
  int length = vfn.size();
  array_face_normalization = new float[length];
  for (int i=0; i<length; i++) array_face_normalization[i] = vfn[i];
  return array_face_normalization;
}

float* GraphicObject::getPointNormalization() {
  vector<float> vpn;
  for (int i=0; i<vertex_num; i++) {
    vpn.push_back(optimal_results[i].x);  
    vpn.push_back(optimal_results[i].y);  
    vpn.push_back(optimal_results[i].z);  
  }
  int length = vpn.size();
  array_point_normalization = new float[length];
  for (int i=0; i<length; i++) {
    array_point_normalization[i] = vpn[i];
    cout << array_point_normalization[i];
  }
  return array_point_normalization;
}

float** GraphicObject::getNormalization() { // the same as getPontNormalization, but 2D
  array_normalization = new float*[vertex_num];
  for (int i=0; i<vertex_num; i++) array_normalization[i] = new float[3];
  for (int i=0; i<vertex_num; i++) {
    array_normalization[i][0] = optimal_results[i].x;  
    array_normalization[i][1] = optimal_results[i].y;  
    array_normalization[i][2] = optimal_results[i].z;  
  }
  return array_normalization;
}

void GraphicObject::test() {
  /*
  // check if the content read from the file is correct
  cout << "x in first vertex element: " << properties[0].x << endl;
  int size = faces.size() - 1;
  cout << "z in last face element: " << faces[size].z << endl; 
  //calculateVectors();
  for (int i=0; i<vertex_num; i++) 
    cout << "(" << optimal_results[i].x << ", " << optimal_results[i].y << ", " << optimal_results[i].z << ")" << endl;
  cout << "test properties: " << endl;
  float* array = getProperties();
  for (int i=0; i<vertex_num*3; i++) {
    cout << array[i] << endl;
  }*/
  //cout << array_face_normalization[0] << endl;
  //cout << array_point_normalization[0] << endl;
  //for (int i=0; i<element_face_num; i++) cout << array_face_normalization[i] << endl;
  //for (int i=0; i<vertex_num*3; i++) cout << array_point_normalization[i] << endl;
}
