//
//  GraphicCamera.h
//  Graphic
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//
//  Author: cosmobserver@gmail.com (Yanxiang Wu <Julian>)
//
//  This file contains a camera that can be adjusted with mouse
//
//  Created on 03/01/13.
//
#include "GLCommonHeader.h"
#include "GraphicCamera.h"

#include <stdio.h>
namespace GraphicCamera {
  
static const int kINACTIVE = 0;
static const int kTRANSLATE = 1;
static const int kROTATE = 2;
static const int kZOOM = 3;
int MouseStartX, MouseStartY, MousePrevX, MousePrevY;

static const float kEPSILON = 1e-5;

inline double Rad2Deg(double rad) { return rad / kPI * 180.0; }
inline double Deg2Rad(double deg) { return deg / 180 * kPI;}



// GraphicCamera::GraphicCamera() : 
//   fru_(60, 1.333,0.02, 100){
//   this->focus_ = 1;
//   pos_ = new vec3d(0.0, 1.0, 2.0);
//   aim_ = new vec3d(0.0, 0.0, 0.0);
//   up_  = new vec3d(0.0, 1.0, 0.0);

// }



// Constructor with position, aim, and up vector
GraphicCamera::GraphicCamera(const float* pos, 
                             const float* aim, const float* up, double focus) :
                             fru_(60, 1.333, 0.02, 100){

  vec3d zaixs(pos[0] - aim[0], pos[1] - aim[1], pos[2] - aim[2]);
  // zaixs.print_v();
  vec3d nzaixs(-zaixs.x, -zaixs.y, -zaixs.z);
  // nzaixs.print_v();
  vec3d dir = nzaixs.Normalization();
  vec3d _up = (new vec3d(up[0], up[1], up[2]))->Normalization();
  vec3d xaxis = dir.cross(_up);
  if (xaxis.Norm() < kEPSILON ) {
    fprintf(stderr, "Error xaxis: %f %f %f\n", 
      zaixs.x, zaixs.y, zaixs.z);
      //xaxis.x, xaxis.y, xaxis.z);
    throw "Up parallel to aim. Wrong parameter";
  }

  this->pos_ = *(new vec3d(pos));
  this->aim_ = *(new vec3d(aim));
  this->up_ = xaxis.Normalization().cross(dir); // make it always perpendicular
                                        // to dir
  this->focus_ = focus;
}


// // Constructor with position, aim, and up vercor as well as the
// //  clipping plane and view angle (FoV)
// GraphicCamera::GraphicCamera::GraphicCamera(const Vec3d& pos, 
//     const Vec3d& aim, const Vec3d up, float near, float far, 
//                                             float ViewAngle,
//                                             double focus) :
//     fru_(ViewAngle, 1.333, near, far){
//   this->GetCoordinate(pos, aim, up);
//   this->focus_ = focus;
//   Init();
// }

// Set up perspective display with this camera
void GraphicCamera::PerspectiveDisplay(int width, int height) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  gluPerspective(fru_.FoV(), (float) width/ (float) height,
                 fru_.Near(), fru_.Far());
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(pos_.x, pos_.y, pos_.z,
            aim_.x, aim_.y, aim_.z,
            up_.x,  up_.y,  up_.z);
}

const GraphicCamera& GraphicCamera::operator=(const GraphicCamera& cam) {
  aim_ = cam.aim_;
  pos_ = cam.pos_;
  up_ = cam.up_;
  return *this;
}

//=====================Below is the implementation of AA and DoF===============
// These data comes from OpenGL Programming Guide
// http://www.glprogramming.com/red/chapter10.html
double jitterTable4[] = {
  0.375, 0.25, 0.125, 0.75, 0.875, 0.25, 0.625, 0.75
};

double jitterTable8[] = {
  0.5625, 0.4375, 0.0625, 0.9375, 0.3125, 0.6875, 0.6875, 0.8124,
  0.8125, 0.1875, 0.9375, 0.5625, 0.4375, 0.0625, 0.1875, 0.3125
};

double jitterTable12[] = {
  0.4166666666, 0.625, 0.9166666666, 0.875, 0.25, 0.375,
  0.4166666666, 0.125, 0.75, 0.125, 0.0833333333, 0.125, 0.75, 0.625,
  0.25, 0.875, 0.5833333333, 0.375, 0.9166666666, 0.375,
  0.0833333333, 0.625, 0.583333333, 0.875
};

double jitterTable16[] = {
  0.375, 0.4375, 0.625, 0.0625, 0.875, 0.1875, 0.125, 0.0625,
  0.375, 0.6875, 0.875, 0.4375, 0.625, 0.5625, 0.375, 0.9375,
  0.625, 0.3125, 0.125, 0.5625, 0.125, 0.8125, 0.375, 0.1875,
  0.875, 0.9375, 0.875, 0.6875, 0.125, 0.3125, 0.625, 0.8125
};

static inline double* GetJitterTable(int level) {
  if (level == 4) return jitterTable4;
  if (level == 8) return jitterTable8;
  if (level == 12)
    return jitterTable12;
  else
    return jitterTable16;
}
  
void GraphicCamera::AAPerspectiveDisplay(int width, int height,
                                         int level,
                                         render_callback render_frame) {
  float time_factor = 1.0;
//  cout<<"Do AA at level:"<<level<<endl;
  double* jitterTable = GetJitterTable(level);
  glClearAccum(0, 0, 0, 0);
  glClear(GL_ACCUM_BUFFER_BIT);
  for (int i = 0 ; i < level; ++i) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    JitterCamera(jitterTable[i*2 + 0] * time_factor,
                 jitterTable[i*2 + 1] * time_factor);
    render_frame();
    glAccum(GL_ACCUM, 1.0/(float)level);
  }
  glAccum(GL_RETURN, 1.0);
  
}
  
void GraphicCamera::DoFPerspectiveDisplay(int width, int height,
                                            int blur_level,
                                            render_callback render_frame) {
    glClearAccum(0, 0, 0, 0);
    glClear(GL_ACCUM_BUFFER_BIT);
    float accu_times = blur_level * blur_level;
    for (int xt = -blur_level/2; xt < blur_level/2; xt ++) {
      for (int yt = -blur_level/2; yt < blur_level/2; yt ++) {
        JitterCamera(0.0, 0.0,
                     (float)xt / (1000 * blur_level / 4),
                     (float)yt / (1000 * blur_level / 4),//yt,
                     this->focus()
                     );
        render_frame();
        glAccum(GL_ACCUM, 1.0/accu_times);
      }
    }
    glAccum(GL_RETURN, 1.0);
  }

void GraphicCamera::JitterCamera(GLfloat pix_x, GLfloat pix_y,
                                    GLfloat eye_x, GLfloat eye_y,
                                    GLdouble focus){
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
  GLfloat frustumWidth= fru_.Right() - fru_.Left();
  GLfloat frustumHeight= fru_.Top() - fru_.Bottom();
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  GLfloat dx, dy;
  dx = - (pix_x * frustumWidth/wndWidth + eye_x * fru_.Near() / focus);
  dy = - (pix_y * frustumHeight/WndHeight + eye_y * fru_.Near() / focus);
  glFrustum(fru_.Left() + dx, fru_.Right() + dx, fru_.Bottom() + dy,
            fru_.Top() + dy, fru_.Near(), fru_.Far() );
  glMatrixMode(GL_MODELVIEW);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(pos_.x + eye_x, pos_.y + eye_y, pos_.z,
            aim_.x, aim_.y, aim_.z,
            up_.x,  up_.y,  up_.z);
}

// jitter a camera by pixels in x and y
void GraphicCamera::JitterCamera(GLfloat pix_x, GLfloat pix_y) {
  JitterCamera(pix_x, pix_y, 0.0f, 0.0f, 1.0f);
}

  
//============================Frustrum=====================================
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
    
    this->fov = Rad2Deg(atan((top - bottom) / 2.0 / near) ) * 2;
  }
  
  Frustum::Frustum(float fovy, float aspect, float near, float far){
    double f = tan(Deg2Rad(fovy / 2));
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
    this->fov = fovy;
  }
  
  
  inline double& Frustum::Left() { return this->left; }
  
  inline double& Frustum::Right() { return this->right; }
  
  inline double& Frustum::Top(){ return this->top; }
  
  inline double& Frustum::Bottom(){ return this->bottom; }
  
  inline double& Frustum::Near(){ return this->near; }
  
  inline double& Frustum::Far(){ return this->far; }
  
  inline double& Frustum::FoV() { return  this->fov; }
  
  double* Frustum::GetMatrix(){ return this->matrix; }
  
} //ns GraphicCamera
