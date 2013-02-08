//
//  GraphicUtilities.cpp
//  Graphic
//
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//
//  Author: cosmobserver@gmail.com (Yanxiang Wu <Julian>)
//
//  Created on 1/31/13
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
    JitterCamera(jitterTable[i][0], jitterTable[i][1], fru);
    render_frame();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glAccum(GL_ACCUM, 1.0/(float)level);
  }
  glAccum(GL_RETURN, 1.0);
}

// jitter a camera by pixels in x and y
void GraphicUtilities::JitterCamera(GLfloat pix_x, GLfloat pix_y, Frustum* fru) {
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

// This function comes from the code of Songho's works:
//    http://www.songho.ca/opengl/gl_transform.html#modelview
// I use this as debug helper functions that help me better understand
// about the OpenGL frustum works.
// It should not be used in the assignment
void Frustum::DrawFrustum(float fovY, float aspectRatio, float nearPlane, float farPlane){
  float tangent = tanf(fovY/2 * 3.141593f / 180.0);
  float nearHeight = nearPlane * tangent;
  float nearWidth = nearHeight * aspectRatio;
  float farHeight = farPlane * tangent;
  float farWidth = farHeight * aspectRatio;
  
  // compute 8 vertices of the frustum
  float vertices[8][3];
  // near top right
  vertices[0][0] = nearWidth;     vertices[0][1] = nearHeight;    vertices[0][2] = -nearPlane;
  // near top left
  vertices[1][0] = -nearWidth;    vertices[1][1] = nearHeight;    vertices[1][2] = -nearPlane;
  // near bottom left
  vertices[2][0] = -nearWidth;    vertices[2][1] = -nearHeight;   vertices[2][2] = -nearPlane;
  // near bottom right
  vertices[3][0] = nearWidth;     vertices[3][1] = -nearHeight;   vertices[3][2] = -nearPlane;
  // far top right
  vertices[4][0] = farWidth;      vertices[4][1] = farHeight;     vertices[4][2] = -farPlane;
  // far top left
  vertices[5][0] = -farWidth;     vertices[5][1] = farHeight;     vertices[5][2] = -farPlane;
  // far bottom left
  vertices[6][0] = -farWidth;     vertices[6][1] = -farHeight;    vertices[6][2] = -farPlane;
  // far bottom right
  vertices[7][0] = farWidth;      vertices[7][1] = -farHeight;    vertices[7][2] = -farPlane;
  
  float colorLine1[4] = { 0.7f, 0.7f, 0.7f, 0.7f };
  float colorLine2[4] = { 0.2f, 0.2f, 0.2f, 0.7f };
  float colorPlane[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
  
  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  // draw the edges around frustum
  glBegin(GL_LINES);
  glColor4fv(colorLine2);
  glVertex3f(0, 0, 0);
  glColor4fv(colorLine1);
  glVertex3fv(vertices[4]);
  
  glColor4fv(colorLine2);
  glVertex3f(0, 0, 0);
  glColor4fv(colorLine1);
  glVertex3fv(vertices[5]);
  
  glColor4fv(colorLine2);
  glVertex3f(0, 0, 0);
  glColor4fv(colorLine1);
  glVertex3fv(vertices[6]);
  
  glColor4fv(colorLine2);
  glVertex3f(0, 0, 0);
  glColor4fv(colorLine1);
  glVertex3fv(vertices[7]);
  glEnd();
  
  glColor4fv(colorLine1);
  glBegin(GL_LINE_LOOP);
  glVertex3fv(vertices[4]);
  glVertex3fv(vertices[5]);
  glVertex3fv(vertices[6]);
  glVertex3fv(vertices[7]);
  glEnd();
  
  glColor4fv(colorLine1);
  glBegin(GL_LINE_LOOP);
  glVertex3fv(vertices[0]);
  glVertex3fv(vertices[1]);
  glVertex3fv(vertices[2]);
  glVertex3fv(vertices[3]);
  glEnd();
  
  // draw near and far plane
  glColor4fv(colorPlane);
  glBegin(GL_QUADS);
  glVertex3fv(vertices[0]);
  glVertex3fv(vertices[1]);
  glVertex3fv(vertices[2]);
  glVertex3fv(vertices[3]);
  glVertex3fv(vertices[4]);
  glVertex3fv(vertices[5]);
  glVertex3fv(vertices[6]);
  glVertex3fv(vertices[7]);
  glEnd();
  
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
}

// A function that used to draw a grid that centered at 0 0 0 so that it
// showes the 'world plane' better
void GraphicUtilities::DrawGrid(float size, float step) {
  // disable lighting
  glDisable(GL_LIGHTING);
  
  glBegin(GL_LINES);
  
  glColor3f(0.5f, 0.5f, 0.5f);
  for(float i = step; i <= size; i+= step)
  {
    glVertex3f(-size, 0,  i);   // x axis paralled lines
    glVertex3f( size, 0,  i);
    glVertex3f(-size, 0, -i);
    glVertex3f( size, 0, -i);
    
    glVertex3f( i, 0, -size);   // z axis parralled lines
    glVertex3f( i, 0,  size);
    glVertex3f(-i, 0, -size);
    glVertex3f(-i, 0,  size);
  }
  
  // x-axis
  glColor3f(1.0f, 0, 0);
  glVertex3f(-size, 0, 0);
  glVertex3f( size, 0, 0);
  
  // z-axis
  glColor3f(0,0,1.0f);
  glVertex3f(0, 0, -size);
  glVertex3f(0, 0,  size);
  
  glEnd();
  
  // enable lighting back
  glEnable(GL_LIGHTING);
}


inline float Camera::near_plane() { return _near_plane; }

inline float Camera::far_plane() { return  _far_plane; }

inline float Camera::fov() { return _fov; }


