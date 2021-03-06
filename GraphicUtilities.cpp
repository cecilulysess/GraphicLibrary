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

namespace GraphicUtilities{

char* GraphicUtilities::read_shader_program(const char *filename){
  FILE *fp;
  char *content = NULL;
  int fd, count;
  fd = open(filename, O_RDONLY);
  count = (int) lseek(fd, 0, SEEK_END);
  close(fd);
  content = (char *) calloc(1, (count + 1));
  fp = fopen(filename, "r");
  count = (int) fread(content, sizeof(char), count, fp);
  content[count] = '\0';
  fclose(fp);
  return content;
}

// These data comes from OpenGL Programming Guide
// http://www.glprogramming.com/red/chapter10.html
Matrixd jitterTable4 = {
  {0.375, 0.25}, {0.125, 0.75}, {0.875, 0.25}, {0.625, 0.75}
};

Matrixd jitterTable8 = {
  {0.5625, 0.4375},
  {0.0625, 0.9375},
  {0.3125, 0.6875},
  {0.6875, 0.8124},
  {0.8125, 0.1875},
  {0.9375, 0.5625},
  {0.4375, 0.0625},
  {0.1875, 0.3125}
};

Matrixd jitterTable12 = {
  {0.4166666666, 0.625}, {0.9166666666, 0.875}, {0.25, 0.375},
  
  {0.4166666666, 0.125}, {0.75, 0.125}, {0.0833333333, 0.125}, {0.75, 0.625},
  
  {0.25, 0.875}, {0.5833333333, 0.375}, {0.9166666666, 0.375},
  
  {0.0833333333, 0.625}, {0.583333333, 0.875}
};

Matrixd jitterTable16 = {
  {0.375, 0.4375}, {0.625, 0.0625}, {0.875, 0.1875}, {0.125, 0.0625},
  
  {0.375, 0.6875}, {0.875, 0.4375}, {0.625, 0.5625}, {0.375, 0.9375},
  
  {0.625, 0.3125}, {0.125, 0.5625}, {0.125, 0.8125}, {0.375, 0.1875},
  
  {0.875, 0.9375}, {0.875, 0.6875}, {0.125, 0.3125}, {0.625, 0.8125}
};

static Matrixd& GetJitterTable(int level) {
  if (level == 4) return jitterTable4;
  if (level == 8) return jitterTable8;
  if (level == 12)
    return jitterTable12;
  else
    return jitterTable16;
}


void GraphicUtilities::AntiAlias(int level, render_callback render_frame,
                                 Frustum* fru){
  float time_factor = 1.0;
  cout<<"Do AA at level:"<<level<<endl;
  Matrixd& jitterTable = GetJitterTable(level);
  glClearAccum(0, 0, 0, 0);
  glClear(GL_ACCUM_BUFFER_BIT);
  for (int i = 0 ; i < level; ++i) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    JitterCamera(jitterTable.get(i, 0) * time_factor,
                 jitterTable.get(i, 1) * time_factor, fru);
    render_frame();
    glAccum(GL_ACCUM, 1.0/(float)level);
  }
  glAccum(GL_RETURN, 1.0);
  
}

void GraphicUtilities::DoFScene(render_callback render_frame, Frustum* fru,
              double focus, int blur_level) {
 
  glClearAccum(0, 0, 0, 0);
  glClear(GL_ACCUM_BUFFER_BIT);
  float accu_times = blur_level * blur_level;
  for (int xt = -blur_level/2; xt < blur_level/2; xt ++) {
    for (int yt = -blur_level/2; yt < blur_level/2; yt ++) {
      JitterCamera(0.0, 0.0,
                   (float)xt / (1000 * blur_level / 4),
                   (float)yt / (1000 * blur_level / 4),//yt,
                   focus,
                   fru);
      render_frame();
      glAccum(GL_ACCUM, 1.0/accu_times);
    }
  }
  
  glAccum(GL_RETURN, 1.0);
//  glFlush();
}

void GraphicUtilities::JitterCamera(GLfloat pix_x, GLfloat pix_y,
                                    GLfloat eye_x, GLfloat eye_y,
                                    GLdouble focus,
                                    Frustum* fru){
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
  GLfloat frustumWidth= fru->Right() - fru->Left();
  GLfloat frustumHeight= fru->Top() - fru->Bottom();
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  GLfloat dx, dy;
  dx = - (pix_x * frustumWidth/wndWidth + eye_x * fru->Near() / focus);
//  dx = (eye_x - fru->Near()/focus * eye_x);
  dy = - (pix_y * frustumHeight/WndHeight + eye_y * fru->Near() / focus);
  //  printf("world delta = %f %f\n",dx,dy);q
  glFrustum(fru->Left() + dx, fru->Right() + dx, fru->Bottom() + dy, fru->Top() + dy, fru->Near(), fru->Far() );
  glMatrixMode(GL_MODELVIEW);
//  glPushMatrix();
  //Problematic to do a view translation here, it will remove the materials.
  glLoadIdentity();
  glRotated(0, 0, 0, 1);
  glRotated(10, 1, 0, 0);
  glRotated(0, 0, 1, 0);
  glTranslated(0, -0.15, -0.4);
//  cout<<"Translate Eye:"<<eye_x <<", "<<eye_y<<endl;
  glTranslatef(-eye_x, -eye_y, 0.0);
//  glPopMatrix();
}

// jitter a camera by pixels in x and y
void GraphicUtilities::JitterCamera(GLfloat pix_x, GLfloat pix_y, Frustum* fru) {
  JitterCamera(pix_x, pix_y, 0.0f, 0.0f, 1.0f, fru);
}

Frustum::Frustum(float left, float right, float bottom, float top,
                 float near, float far){
  this->near = near;
  this->far = far;
  this->left = left;
  this->right = right;
  this->top = top;
  this->bottom = bottom;
  matrix[0] = 2.0 * near / (right - left);
  matrix[2] = (right + left) / (right - left);
  matrix[5] = 2.0 * near / ( top - bottom);
  matrix[6] = (top + bottom) / (top - bottom);
  matrix[10] = - (far + near) / ( far - near);
  matrix[11] = - 2.0 * (far * near) / (far - near);
  matrix[14] = -1.0;
}

Frustum::Frustum(float fovy, float aspect, float near, float far){
  double f = tan(fovy / 2 * 3.141592653/180.0);
  this->top = near * f;
  this->right = top * aspect;
  this->left = - right;
  this->bottom = - top;
  this->near = near;
  this->far = far;
  matrix[0] = near / right;
  matrix[5] = near / top;
  matrix[10] = - (far + near) / (far - near);
  matrix[11] = -2.0 * (far * near) / (far - near);
  matrix[14] = -1.0;
}


inline double Frustum::Left() {
  return this->left;
}

inline double Frustum::Right() {
  return this->right;
}
  

inline double Frustum::Top(){
  return this->top;
}

inline double Frustum::Bottom(){
  return this->bottom;
}

inline double Frustum::Near(){
  return this->near;
}

inline double Frustum::Far(){
  return this->far;
}

Matrix4d& Frustum::GetMatrix(){
  return this->matrix;
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

bool GraphicUtilities::LoadTexture(char *filename, GLuint &texture){
  FILE *fptr;
  char buf[512], *parse;
  int im_size, im_width, im_height, max_color;
  unsigned char *texture_bytes;

  fptr = fopen(filename, "r");
  if (fptr == NULL) {
    perror("Reading material file with error: ");
    perror(filename);
  }
  fgets(buf, 512, fptr);

  do {
    fgets(buf, 512, fptr);
  } while(buf[0] == '#' || buf[0] == '\n');
  sscanf(buf, "%d %d", &im_width, &im_height);
  fgets(buf, 512, fptr);
  im_size = im_width * im_height;
  texture_bytes = (unsigned char *) calloc(3, im_size);
  fread(texture_bytes, 3, im_size, fptr);
  fclose(fptr);
  
  glBindTexture(GL_TEXTURE_2D, texture /*SetCurrentTexture id*/);
  glTexImage2D(GL_TEXTURE_2D, 
    0, // Mipmap level
    GL_RGB, im_width, im_height, 0/* border size */, GL_RGB,
    GL_UNSIGNED_BYTE, texture_bytes);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  free(texture_bytes);
  printf("Loaded texture: %s\n", filename);
  return true;
}

void GLShortCut::PrintGLErrors(char *file, int line){
  GLenum glErr;
  glErr = glGetError();
//  fprintf(stderr, "Detecting Error...\n");
  if (glErr != GL_NO_ERROR) {
    fprintf(stderr, "glError in file %s @ line %d: %s\n", 
            file, line, gluErrorString(glErr));
  }
}

  
}//GraphicUtilities
