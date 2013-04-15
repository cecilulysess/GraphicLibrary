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

#include <vector>

using std::vector;


class GraphicModel{
public:
  GraphicModel();
  
  // Load model from .obj file
  bool LoadObject(char* file);
  void InitModelData();
  //draw this model in OpenGL
  void DrawModel();
  
  long vert_size() const { return this->vertices.size(); }
  long faces_size() const { return this->faces.size() / face_size; }
  long vert_normal_size() const { return this->vnormal.size() / 3; }

  bool has_vnormal() const {return this->vnormal_idx.size() > 0; }
  bool has_texture() const {return this->texture_idx.size() > 0; }
  
private:
  int face_size ;
  int vertice_size;
  unsigned int GL_draw_buffer_id[3];
  
  vector<float> vertices;
  vector<unsigned int>  faces;
  vector<float> vnormal;
  vector<unsigned int> vnormal_idx;
  vector<unsigned int> texture_idx;

  
};

#endif /* defined(__Graphic__GraphicModel__) */
