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
  int faceidx[4];
  int nidx[4];
  while (fgets(buff, 255, f) != NULL) {
    if (strstr(buff, "#") == buff) {
      printf("This is common\n");
      continue;
    }
    if (strstr(buff, "mtllib") == buff) {
      printf("Object start\n");
      continue;
    }
    
    if (strstr(buff, "v ") == buff) {
      int cnt = sscanf(buff, "v %f %f %f %f", vert, vert+1, vert+2, vert+3);
//      printf("Read %d float: %f %f %f %f\n", cnt, vert[0],
//      vert[1], vert[2], vert[3]);
      for (int i = 0; i < cnt; ++i) {
        this->vertices.push_back(vert[i]);
      }
      continue;
    }
//    printf("vn : %d\n", (strstr(buff, "vn") == NULL));
    if (strstr(buff, "vn") == buff) {
      sscanf(buff, "vn %f %f %f", normal, normal+1, normal+2);
      for (int i = 0 ; i < 3; ++i) {
        this->vnormal.push_back(normal[i]);
      }
      continue;
    }
    if (strstr(buff, "usemtl") == buff) {
      printf("Use mtl\n");
    }
    if (strstr(buff, "f ") == buff) {
      // if use v//vn fashion
      if (strstr(buff, "//") != NULL){
//        printf("Process: %s", buff);
        int cnt = sscanf(buff, "f %d//%d %d//%d %d//%d %d//%d", faceidx, nidx,
               faceidx+1, nidx+1,
               faceidx+2, nidx+2,
               faceidx+3, nidx+3);
//        printf("Read %d, fi: %d %d %d, ni: %d %d %d\n", cnt,
//               faceidx[0], faceidx[1], faceidx[2],
//               nidx[0], nidx[1], nidx[2]);
      } else {
        // use either v/vt or v/vt/vn
        if (strstr(buff, "/") != NULL) {
          printf("I don't like triple\n");
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
}