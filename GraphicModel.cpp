//
//  GraphicModel.cpp
//  Graphic models
//    Copyright (c) 2013 Julian Wu. All rights reserved.
//
//  Released under Apache 2.0 listence
//  
//  Author: cosmobserver@gmail.com (Yanxiang Wu <Julian> )
//  
//  Created on 4/8/13.
//
//

#include "GraphicModel.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include<iostream>
using namespace std;
#include "GraphicUtilities.h"
using GraphicUtilities::GLShortCut;
using GraphicUtilities::GraphicUtilities;


// validate that every normal was used
bool validate_vnormal(vector<unsigned int> vn, int upper) {
  bool *tab = new bool[upper];
  for (int i = 0; i < upper; ++i) {
    tab[i] = false;
  }
  for (int i = 0; i < vn.size(); ++i) {
    tab[vn[i]] = true;
  }
  for (int i = 0; i < upper; ++i) {
    if(tab[i] == false) {
      printf("Vertex Nomal: %d is not used\n", i);
      return false;
    }
  }
  return true;
}

bool GraphicModel::LoadMaterial(char *file){
  if (file == NULL) {
    return false;
  }
  FILE *f;
  f = fopen(file, "r");
  if (f == NULL) {
    perror("Reading material file with error: ");
    perror(file);
  }
  char buff[255];
  while(fgets(buff, 255, f) != NULL) {
    if (strstr(buff, "#") == buff) continue; //skip commments
    if (strstr(buff, "newmtl") == buff) continue; //currently useless
    if (strstr(buff, "Ns ") == buff) {
      sscanf(buff, "Ns %f", &this->material.Shininess);
    }
    if (strstr(buff, "Ka ") == buff) {
      sscanf(buff, "Ka %f %f %f", &this->material.Ambient[0],
                                  &this->material.Ambient[1],
                                  &this->material.Ambient[2]);
    }
    if (strstr(buff, "Kd ") == buff) {
      sscanf(buff, "Kd %f %f %f", &this->material.Diffuse[0],
                                  &this->material.Diffuse[1],
                                  &this->material.Diffuse[2]);
    }
    if (strstr(buff, "Ks ") == buff) {
      sscanf(buff, "Ks %f %f %f", &this->material.Specular[0],
                                  &this->material.Specular[1],
                                  &this->material.Specular[2]);
    }
    if (strstr(buff, "map_Kd ") == buff) {
      char texturefile[255];
      sscanf(buff, "map_Kd %s", texturefile);
      printf("Load Material from:%s\n", texturefile);
      assert(
        GraphicUtilities::GraphicUtilities::LoadTexture(
          texturefile, GL_texture_id[0]) );
    }
    if (strstr(buff, "map_normal ") == buff) {
      char normalfile[255];
      sscanf(buff, "map_normal %s", normalfile);
      printf("Load Normal from:%s\n", normalfile);
      assert(
        GraphicUtilities::GraphicUtilities::LoadTexture(
          normalfile, GL_texture_id[1]) );
    }
  }
}

bool GraphicModel::LoadObject(char *file) {
  if (file == NULL) {
    return false;
  }
  FILE *f;
  f = fopen(file, "r");
  char buff[255];
  if (f == NULL) {
    perror("Reading object file with error: ");
  }
  int vn_cnt = 0, v_cnt = 0, vt_cnt = 0, vx_cnt = 0, vy_cnt = 0;
  float vert[4];
  float normal[3];
  float text_map[2];
  float tangentnormal[3];
  unsigned int faceidx[4];
  unsigned int nidx[4];
  unsigned int textidx[4];
  vector<float> vertex;
  vector<float> vert_norm;
  vector<float> text_map_ori;
  while (fgets(buff, 255, f) != NULL) {
    if (strstr(buff, "#") == buff) continue;  // skip comments
    if (strstr(buff, "o ") == buff) continue; // new object start
    if (strstr(buff, "usemtl") == buff) continue; // using some material
    if (strstr(buff, "mtllib") == buff) {
      char data[128];
      sscanf(buff, "mtllib %s", data);
      LoadMaterial(data);
      continue;
    }
    
    if (strstr(buff, "v ") == buff) {
      int cnt = sscanf(buff, "v %f %f %f %f",
                                  vert, vert+1, vert+2, vert+3);
      this->vertice_size = cnt;
      assert(vertice_size >= 3 && vertice_size <=4);
      for (int i = 0; i < vertice_size; ++i) {
        vertex.push_back(vert[i]);
      }
      v_cnt ++;
      continue;
    }
    if (strstr(buff, "vn") == buff) {
      sscanf(buff, "vn %f %f %f", normal, normal+1, normal+2);
      for (int i = 0 ; i < 3; ++i) {
        vert_norm.push_back(normal[i]);
      }
      vn_cnt ++;
      continue;
    }
    if (strstr(buff, "vt") == buff) {
      sscanf(buff, "vt %f %f", text_map, text_map + 1);
      for (int i = 0 ; i < 2; ++i) {
        text_map_ori.push_back(text_map[i]);
      }
      vt_cnt ++;
      continue;
    }
    if (strstr(buff, "vx ") == buff) {
      sscanf(buff, "vx %f %f %f", tangentnormal, tangentnormal + 1,
                                tangentnormal + 2);
      for (int i = 0 ; i < 3; ++i) {
        tengent.push_back(tangentnormal[i]);
      }
      vx_cnt ++;
      continue;
    }
    if (strstr(buff, "vy ") == buff) {
      sscanf(buff, "vy %f %f %f", tangentnormal, tangentnormal + 1,
                                  tangentnormal + 2);
      for (int i = 0 ; i < 3; ++i) {
        bitengent.push_back(tangentnormal[i]);
      }
      vy_cnt ++;
      continue;
    }

    if (strstr(buff, "f ") == buff) {
      // if use v//vn fashion
      if (strstr(buff, "//") != NULL){
        int cnt = sscanf(buff, "f %d//%d %d//%d %d//%d %d//%d", 
               faceidx, nidx,
               faceidx+1, nidx+1,
               faceidx+2, nidx+2,
               faceidx+3, nidx+3);
        if (cnt < 4) {
          printf("Find Triangular face. Current only support quad face.\n");
          assert(0);
        }
        for(int i = 0; i < 4; ++i) {
          this->faces.push_back(faceidx[i] - 1);
          // fprintf(stderr, "Current faces %d: %d\n", faces.size(), faces[faces.size()-1]);
          //this->vnormal.push_back(nidx[i] - 1);
          this->vnormal_idx.push_back(nidx[i] - 1);
        }
      } else {
        // use either v/vt or v/vt/vn
        if (strstr(buff, "/") != NULL) {
          int cnt = sscanf(buff, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", 
               faceidx, textidx, nidx,
               faceidx+1, textidx + 1, nidx+1, 
               faceidx+2, textidx + 2, nidx+2, 
               faceidx+3, textidx + 3, nidx+3 );
      
          for(int i = 0; i < 4; ++i) {
            this->faces.push_back(faceidx[i] - 1);
            this->texture_idx.push_back(textidx[i] - 1);
            if (cnt > 8) {
              this->vnormal_idx.push_back(nidx[i] - 1);
            }
          }
        } else {
          // use only v v v fashion
          printf("I don't like it\n");
          exit(1);
        }
      }
    }

  }

  printf("Load raw data completed\n\tLoad vertex: %d, texture map: %d, vertex normal: %d\n",
          v_cnt, vt_cnt, vn_cnt);
  printf("\tFaces: %d, VNormal Index: %d, VTengent: %d, VBitengent: %d\n", 
          this->faces_size(), vnormal_idx.size(), tengent.size() / 3,
          bitengent.size() / 3 );
  assert(vt_cnt > 0);
  for (int i = 0; i < faces.size(); ++i){
    this->vertices.push_back(vertex[faces[i] * 3 + 0]);
    this->vertices.push_back(vertex[faces[i] * 3 + 1]);
    this->vertices.push_back(vertex[faces[i] * 3 + 2]);
    this->vnormal.push_back(vert_norm[vnormal_idx[i] * 3 + 0]);
    this->vnormal.push_back(vert_norm[vnormal_idx[i] * 3 + 1]);
    this->vnormal.push_back(vert_norm[vnormal_idx[i] * 3 + 2]);
    this->texture_mapping.push_back(text_map_ori[texture_idx[i] * 2]);

    this->texture_mapping.push_back(text_map_ori[texture_idx[i] * 2 + 1]);
  }

  assert(this->vertices.size() == 3 * faces.size());
  assert(this->vnormal.size() == 3 * faces.size());
  assert(this->texture_mapping.size() == 2 * faces.size());

  int idx = faces.size();
  faces.clear();
  for (int i = 0; i < idx; ++i)
  {
    faces.push_back(i);
  }
  
  return true;
}
GraphicModel::GraphicModel(){
  glGenBuffers(4, GL_draw_buffer_id);
  glGenTextures(2, GL_texture_id);
  this->face_size = 4;
  this->vertice_size = 3;
}


void GraphicModel::InitModelData(int shader_id) {

  //bind vertex array
  glBindBuffer(GL_ARRAY_BUFFER, GL_draw_buffer_id[0]);
  
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices.size(),
               &this->vertices[0], GL_STATIC_DRAW);
  GLShortCut::PrintGLErrors(__FILE__, __LINE__);
  
  glVertexPointer(vertice_size, GL_FLOAT, 3 * sizeof(float), (void*)0);
  // bind face index array
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_draw_buffer_id[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->faces.size(),
               &this->faces[0], GL_STATIC_DRAW);
  // bind vertex normal array
  
  glBindBuffer(GL_ARRAY_BUFFER, GL_draw_buffer_id[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vnormal.size(),
              &this->vnormal[0], GL_STATIC_DRAW);
  glNormalPointer( GL_FLOAT, 3 * sizeof(float), (void*)0);

  // bind textrue mapping array
  
  glBindBuffer(GL_ARRAY_BUFFER, GL_draw_buffer_id[3]);
  glBufferData(GL_ARRAY_BUFFER, 
               sizeof(float) * this->texture_mapping_size(),
               &this->texture_mapping[0], GL_STATIC_DRAW);
  glTexCoordPointer(2, GL_FLOAT, 0, (void*)0);


}

void GraphicModel::DrawModel(int draw_parameter, int shader_id) {
  glEnable(GL_DEPTH_TEST);
  glClearColor(0,0,0,0);
  // do materials for this model
  glMaterialfv(GL_FRONT, GL_AMBIENT, material.Ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, material.Diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, material.Specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, &material.Shininess);
  // ============ Enable states=================
  // glClientActiveTexture(GL_TEXTURE0);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, GL_texture_id[0]);
  // ==========================================
  // 
  // int attribute_texcoord = glGetAttribLocation(shader_id, "texcoord");
  // glEnableVertexAttribArray(attribute_texcoord);
  // glVertexAttribPointer(
  //   attribute_texcoord, // attribute
  //   2,                  // number of elements per vertex, here (x,y)
  //   GL_FLOAT,           // the type of each element
  //   GL_FALSE,           // take our values as-is
  //   0,                  // no extra data between each position
  //   0                   // offset of first element
  // );

  
  //this->faces_size()
  for (int i = 0; i < 1; i ++) {
//    glNormal3fv(&vnormal[this->vnormal_idx[i * 3]]);\
    //(int) this->faces_draw_size()
    // glTexCoord2fv(&this->texture_mapping[i * 4 * 2 + 0]);
    glDrawElements(GL_QUADS, (int) this->faces_draw_size() ,
                  GL_UNSIGNED_INT, (void*)(i * 4 * 4)  );
    //fprintf(stderr, "model drawed\n");
  }
  printf("Drawed model, faces:%d\n", this->faces_draw_size());


 // ==============Disable States===============
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  // glDisableVertexAttribArray(attribute_texcoord);
  glDisable(GL_TEXTURE_2D);
 // ===========================================

  if (draw_parameter){
   glUseProgram(0);
   glDisable(GL_LIGHTING);
   glClearColor(0,0,0,0);
  //  float* v = normal;
   float* vn = &vnormal[0];
   float div = 10.0;
   glColor4f(0, 0, 1.0, 1.0);
   for (int i = 0; i < vnormal.size() / 3 ; i++) { //
     glBegin(GL_LINES);
       glVertex3fv(&vertices[i * 3]);
       glVertex3f(vertices[i * 3] + vn[i * 3] / div ,
            vertices[i * 3 + 1] + vn[i * 3 + 1] / div,
            vertices[i * 3 + 2] + vn[i * 3 + 2] / div);
     glEnd();
   }
   

   glEnable(GL_LIGHTING);
  }
}