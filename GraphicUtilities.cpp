//
//  GraphicUtilities.cpp
//  Graphic
//
//  Created by Julian Wu on 1/31/13.
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//
#include "GLCommonHeader.h"
#include "GraphicUtilities.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>


char* GraphicUtilities::read_shader_program(char *filename){
  FILE *fp;
  char *content = NULL;
  int fd, count;
  fd = open(filename, O_RDONLY);
  count = lseek(fd, 0, SEEK_END);
  close(fd);
  content = (char *) calloc(1, (count + 1));
  fp = fopen(filename, "r");
  count = fread(content, sizeof(char), count, fp);
  content[count] = '\0';
  fclose(fp);
  return content;
}

float jitterTable[][2] = {
  {0.5625, 0.4375},
  {0.0625, 0.9375},
  {0.3125, 0.6875},
  {0.6875, 0.8124},
  {0.8125, 0.1875},
  {0.9375, 0.5625},
  {0.4375, 0.0625},
  {0.1875, 0.3125}
};

void GraphicUtilities::AntiAlias(int level, render_callback render_frame, Frustum* fru){
  glClearAccum(0, 0, 0, 0);
  glClear(GL_ACCUM_BUFFER_BIT);
  for (int i = 0 ; i < level; ++i) {
    jitterCamera(jitterTable[i][0], jitterTable[i][1], fru);
    render_frame();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glAccum(GL_ACCUM, 1.0/(float)level);
  }
  glAccum(GL_RETURN, 1.0);
}

// jitter a camera by pixels in x and y
void GraphicUtilities::jitterCamera(GLfloat pix_x, GLfloat pix_y, Frustum* fru) {
  GLint viewport_params[4];
  // from OpenGL doc: When used with non-indexed variants of glGet
  //  (such as glGetIntegerv), params returns four values: the x
  //  and y window coordinates of the viewport, followed by its
  //  width and height. Initially the  x and y window coordinates
  //  are both set to 0, and the width and height are set to the
  //  width and height of the window into which the GL will do its rendering
  glGetIntegerv(GL_VIEWPORT, viewport_params);
  GLfloat wndWidth=viewport_params[2];
  GLfloat WndHeight=viewport_params[3];
  GLfloat frustumWidth=fru->Right() - fru->Left();
  GLfloat frustumHeight=fru->Top() - fru->Bottom();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  GLfloat dx, dy;
  dx = pix_x*frustumWidth/wndWidth;
  dy = pix_y*frustumHeight/WndHeight;
  printf("world delta = %f %f\n",dx,dy);
  glFrustum(fru->Left() + dx, fru->Right() - dx, fru->Bottom() + dy, fru->Top() + dy, fru->Near(), fru->Far() );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

Frustum::Frustum(float near, float far, float left, float right, float bottom, float top){
  this->near = near;
  this->far = far;
  this->left = left;
  this->right = right;
  this->top = top;
  this->bottom = bottom;
  matrix[0] = (2 * near) / (right - left);
  matrix[1] = 0;
  matrix[2] = 0;
  matrix[3] = 0;
  matrix[4] = 0;
  matrix[5] = (2 * near) / (top - bottom);
  matrix[6] = 0;
  matrix[7] = 0;
  matrix[8] = (right + left) / (right - left);
  matrix[9] = (top + bottom) / (top - bottom);
  matrix[10] = - (far + near) / (far - near);
  matrix[11] = -1;
  matrix[12] = 0;
  matrix[13] = 0;
  matrix[14] = - ( 2 * far * near) / (far - near);
  matrix[15] = 0;
}

Frustum::Frustum(float fovy, float aspect, float near, float far){
  this->near = near;
  this->far = far;
  this->top = near * tan(fovy);
  this->bottom = - this->top;
  this->right = top * aspect;
  this->left = - this->right;
  matrix[0] = near/right;
  matrix[1] = 0;
  matrix[2] = 0;
  matrix[3] = 0;
  matrix[4] = 0;
  matrix[5] = near /top;
  matrix[6] = 0;
  matrix[7] = 0;
  matrix[8] = 0;
  matrix[9] = 0;
  matrix[10] = - (far + near) / (far - near);
  matrix[11] = -1;
  matrix[12] = 0;
  matrix[13] = 0;
  matrix[14] = - ( 2 * far * near) / (far - near);
  matrix[15] = 0;
}