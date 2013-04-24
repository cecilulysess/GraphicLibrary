//
//  GraphicModel.h
//  Graphic
//    Copyright (c) 2013 Julian Wu. All rights reserved.
//
//  Released under Apache 2.0 listence
//  
//  Author: cosmobserver@gmail.com (Yanxiang Wu <Julian> )
//  
//  Created on 4/8/13.
//
//

#ifndef __Graphic__GraphicModel__
#define __Graphic__GraphicModel__

#include "GLCommonHeader.h"
#include <stdio.h>
#include <vector>

using std::vector;

// Using a self defined structure
typedef struct Material{
  char name[128]; //name, I don't like pointer here
  float Ambient[3];
  float Diffuse[3];
  float Specular[3];
  float Shininess;

  void print(){
    printf("\tMaterial: %d\n", name);
    printf("\tAmbient:  %f %f %f\n", Ambient[0], Ambient[1], Ambient[2]);
    printf("\tDiffuse:  %f %f %f\n", Diffuse[0], Diffuse[1], Diffuse[2]);
    printf("\tSpecular: %f %f %f\n", Specular[0], Specular[1], Specular[2]);
    printf("\tShininess: %f\n", Shininess);
  }

} Material;

// A graphic object model class that know how to draw itself, how to load
// its material and texture
// Assumptions: current only using 1 texture, 1 normal map, 1 material
//
// Usage: 1. LoadObject will fill in all the data required
//        2. InitModelData will feed it into Graphic Card
//        3. DrawModel within each frame
class GraphicModel{
public:
  GraphicModel(int texture_unit); 

  // Load model from .obj file
  bool LoadObject(char* file);
  void InitModelData(int text_attr_loc);
  //draw this model in OpenGL
  // draw_parameter control draw model specific info such as normal
  void DrawModel(int draw_parameter, int shader_id);
  
  // vertices size (in terms of # of float)
  long vert_size() const { return this->vertices.size(); }
  // texture mapping uv size ( in terms of # of float)
  long texture_mapping_size() const {return this->texture_mapping.size(); }
  // return the faces index that used for glDrawElements
  long faces_draw_size() const {return this->faces.size();}
  long vert_normal_size() const { return this->vnormal.size() / 3; }
  long faces_size() const { return this->faces.size() / face_size; }

  bool has_vnormal() const {return this->vnormal_idx.size() > 0; }
  bool has_texture() const {return this->texture_idx.size() > 0; }
  bool has_normalmap() const {return this->tangent.size() > 0;}
private:
  bool LoadMaterial(char *file);

  int texture_unit;
  int face_size ;
  int vertice_size;
  // vertex position in 0, face vertice idx in 1, vertex normal in 2, texture uv in 3
  // vertex tangent in 4, vertex bitangent in 5
  unsigned int GL_draw_buffer_id[6];
  //texture at 0, normal map at 1
  unsigned int GL_texture_id[2];

  unsigned int GL_VAO_id[1];

  // vertices, note that this is not the v from obj file
  vector<float> vertices;
  vector<float> vnormal;
  vector<float> texture_mapping;
  vector<float> tangent, bitangent;
  vector<unsigned int>  faces;
  vector<unsigned int> vnormal_idx;
  vector<unsigned int> texture_idx;

  Material material;
  
};

#endif /* defined(__Graphic__GraphicModel__) */
