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
    if(tab[i] == false) return false;
  }
  return true;
}
bool GraphicModel::LoadObject(char *file) {
  if (file == NULL) {
    return false;
  }
  FILE *f;
  f = fopen(file, "r");
  char buff[255];
  if (f == NULL) {
    perror("Reading file with error: ");
  }
  float vert[4];
  float normal[3];
  unsigned int faceidx[4];
  unsigned int nidx[4];
  unsigned int textidx[4];
  vector<float> vert_norm;
  while (fgets(buff, 255, f) != NULL) {
    if (strstr(buff, "#") == buff) {
      printf("This is comment\n");
      continue;
    }
    if (strstr(buff, "mtllib") == buff) {
      printf("Object start\n");
      continue;
    }
    
    if (strstr(buff, "v ") == buff) {
      int cnt = sscanf(buff, "v %f %f %f %f",
                                  vert, vert+1, vert+2, vert+3);
      this->vertice_size = cnt;
      assert(vertice_size >= 3 && vertice_size <=4);
      for (int i = 0; i < vertice_size; ++i) {
        this->vertices.push_back(vert[i]);
      }
      continue;
    }
    if (strstr(buff, "vn") == buff) {
      sscanf(buff, "vn %f %f %f", normal, normal+1, normal+2);
      for (int i = 0 ; i < 3; ++i) {
        vert_norm.push_back(normal[i]);
      }
      continue;
    }
    if (strstr(buff, "usemtl") == buff) {
      printf("Use mtl\n");
    }
    if (strstr(buff, "f ") == buff) {
      // if use v//vn fashion
      if (strstr(buff, "//") != NULL){
        int cnt = sscanf(buff, "f %d//%d %d//%d %d//%d %d//%d", 
               faceidx, nidx,
               faceidx+1, nidx+1,
               faceidx+2, nidx+2,
               faceidx+3, nidx+3);
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
            
//            if (faces[faces.size()-1] == 13000) {
//              printf("f/vn : %d %d\n", faceidx[i], nidx[i]);
//            }
          }
        } else {
          // use only v v v fashion
          printf("I don't like it\n");
        }
      }
//      int cnt= sscanf(buff, "f %d/%d/%d", faceidx, faceidx+1, faceidx+2);
//      printf("Face %d : %d %d %d \n", cnt, faceidx[0], faceidx[1], faceidx[2]);
    }
    // otherwise
    // ignore
    //
  }
  float* vnorm = new float[vertices.size()];
  // Change the vertice normal index according to the vertice index,
  // OpenGL doesn't support specifying vert normal seperactly.
  for (int i = 0; i < faces.size(); ++i) {
//    if (faces[i] == 13000) {
//      printf("VN: i:%d %d:%d: %f %f %f\n", i, faces[i], vnormal_idx[i],
//                                      vert_norm[vnormal_idx[i] * 3],
//                                      vert_norm[vnormal_idx[i] * 3 + 1],
//             vert_norm[vnormal_idx[i]* 3 + 2]);
//    }
    vnorm[this->faces[i] * 3] = vert_norm[vnormal_idx[i] * 3 ];
    vnorm[this->faces[i] * 3 + 1] = vert_norm[vnormal_idx[i] * 3 + 1];
    vnorm[this->faces[i] * 3 + 2] = vert_norm[vnormal_idx[i] * 3 + 2];
//    if (faces[i] == 13000) {
//      printf("vnorm: %f %f %f\n", vnorm[this->faces[i] * 3],
//             vnorm[this->faces[i] * 3 + 1],
//             vnorm[this->faces[i] * 3 + 2]);
//    }
  }
  for (int i = 0; i < vertices.size(); ++i ) {
    this->vnormal.push_back(vnorm[i]);
  }
  delete[] vnorm;
  assert(validate_vnormal(vnormal_idx, vnormal.size() / 3));
  if (! (this->vertices.size() == this->vnormal.size()) ) {
    printf("Vert size: %d, VNormal size: %d \n", (int) vertices.size(),
           (int)vnormal.size());
    assert(this->vertices.size() == this->vnormal.size());
  }
  return true;
}
GraphicModel::GraphicModel(){
  glGenBuffers(3, GL_draw_buffer_id);
  this->face_size = 4;
  this->vertice_size = 3;
}

float fvv[] = {
  1.000000,-1.000000,-2.000000,
1.000000,-1.000000,2.000000,
-1.000000,-1.000000,2.000000,
-1.000000,-1.000000,-2.000000,
1.000000,1.000000,-1.999999,
0.999999,1.000000,2.000001,
-1.000000,1.000000,2.000000,
-1.000000,1.000000,-2.000000};

int ffa[] = {1, 2, 3, 4,
5, 8, 7, 6,
1, 5, 6, 2,
2, 6, 7, 3,
3, 7, 8, 4,
5, 1, 4, 8};


void GraphicModel::InitModelData() {
  // for (int i = 0; i < vertices.size(); ++i)
  // {
  //   fprintf(stderr, "%f, ", vertices[i]);
  //   if ((i + 1) % 3 == 0) fprintf(stderr, "\n");
  // }
  fprintf(stderr, "\n");
  fprintf(stderr, "vertices#:%d\n", vertices.size());
  fprintf(stderr, "Vnormal#:%d\n", vnormal.size());
  // for (int i = 0; i < faces.size(); ++i)
  // {
  //   // fprintf(stderr, "%d, ", this->faces[i]);
  //   if ((i + 1) % 4 == 0) fprintf(stderr, "\n");
  // }
  glBindBuffer(GL_ARRAY_BUFFER, GL_draw_buffer_id[0]);
  
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices.size(),
               &this->vertices[0], GL_STATIC_DRAW);
  GLShortCut::PrintGLErrors(__FILE__, __LINE__);
  glVertexPointer(vertice_size, GL_FLOAT, 3 * sizeof(float), (void*)0);
  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_draw_buffer_id[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->faces.size(),
               &this->faces[0], GL_STATIC_DRAW);

  glEnableClientState(GL_NORMAL_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, GL_draw_buffer_id[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vnormal.size(),
              &this->vnormal[0], GL_STATIC_DRAW);
  glNormalPointer( GL_FLOAT, 3 * sizeof(float), (void*)0);
}

void GraphicModel::DrawModel() {
  glEnable(GL_DEPTH_TEST);
  glClearColor(0,0,0,0);
  //this->faces_size()
  for (int i = 0; i < 1; i ++) {
//    glNormal3fv(&vnormal[this->vnormal_idx[i * 3]]);
    glDrawElements(GL_QUADS, (int) this->faces_size() * 4, GL_UNSIGNED_INT, (void*)(i * 4 * 4) );
    //fprintf(stderr, "model drawed\n");
  }

//  glUseProgram(0);
//  glDisable(GL_LIGHTING);
//  glClearColor(0,0,0,0);
// //  float* v = normal;
//  float* vn = &vnormal[0];
//  float div = 10.0;
//  glColor4f(0, 0, 1.0, 1.0);
//  for (int i = 0; i < vnormal.size() / 3 ; i++) { //
//    glBegin(GL_LINES);
//      glVertex3fv(&vertices[i * 3]);
//      glVertex3f(vertices[i * 3] + vn[i * 3] / div ,
//           vertices[i * 3 + 1] + vn[i * 3 + 1] / div,
//           vertices[i * 3 + 2] + vn[i * 3 + 2] / div);
//    glEnd();
//  }
//  
// 
//  glEnable(GL_LIGHTING);
}