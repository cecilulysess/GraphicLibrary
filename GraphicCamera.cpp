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
#include <math.h>

static const int kINACTIVE = 0;
static const int kTRANSLATE = 1;
static const int kROTATE = 2;
static const int kZOOM = 3;
int MouseStartX, MouseStartY, MousePrevX, MousePrevY;

static const float kEPSILON = 1e-5;

inline double Rad2Deg(double rad) { return rad / kPI * 180.0; }
inline double Deg2Rad(double deg) { return deg / 180 * kPI;}

Matrix4d MvMat;
Matrix4d ProjMat;
GLint ViewPort[4];

int CameraMode = kINACTIVE;
Vec3d PrevMousePos;

double dt_elev, dt_azim, loc_dt_elev, loc_dt_azim;

// set camera position, aim, up. Verify, and make up the true up direction
void GraphicCamera::GetCoordinate(const Vec3d& pos, const Vec3d& aim, const Vec3d& up){
  Vec3d zaixs = pos - aim;
  if (zaixs.Norm() < kEPSILON) {
    throw "Camera position and aim are the same. Problematic";
    exit(1);
  }
  Vec3d dir = -zaixs.Normalization();
  Vec3d _up = up.Normalization();
  Vec3d xaxis = dir % _up;
  if (xaxis.Norm() < kEPSILON ) {
    throw "Up parallel to aim. Wrong parameter";
  }

  this->pos_ = pos;
  this->aim_ = aim;
  this->up_ = xaxis.Normalization() % dir; // make it always perpendicular
                                        // to dir
}

void GraphicCamera::Init(){
  Vec3d axis_origin, update_pos;
  Vec3d tmp, tmp1, tmp2;

  default_position = pos_;
  default_aim = aim_;
  default_up = up_;

  // the angle around x axis
  update_pos = pos_ - aim_;
  axis_origin.x() = update_pos.x();
  double dist = (axis_origin - update_pos).Norm();
  
  tmp1.x() = update_pos.x();
  tmp1.z() = dist;
  
  tmp = update_pos.Normalize();
  tmp1 = tmp1.Normalize();

  current_elev = Rad2Deg(acos(tmp * tmp1));
  
  // the angle around y axis
  axis_origin = {0, update_pos.y(), 0};

  dist = (axis_origin - update_pos).Norm();

  tmp2 = {0, update_pos.y(), dist};
  tmp2 = tmp2.Normalize();

  current_azim = 360.0 - Rad2Deg(acos(tmp2 * tmp));

  default_azim = current_azim;
  default_elev = current_elev;
}

GraphicCamera::GraphicCamera() { 
  pos_ = {0.0, 1.0, 2.0};
  aim_ = {0.0, 0.0, 0.0};
  up_  = {0.0, 1.0, 0.0};

  near_ = 0.1;
  far_  = 100;
  fov_  = 60;

  Init();
}

// Constructor with position, aim, and up vector
GraphicCamera::GraphicCamera(const Vec3d& pos, 
   const Vec3d& aim, const Vec3d up) {
  this->GetCoordinate(pos, aim, up);
  near_ = 0.1;
  far_ = 100;
  fov_ = 60.0;

  Init();
}

// Constructor with position, aim, and up vercor as well as the
//  clipping plane and view angle (FoV)
GraphicCamera::GraphicCamera::GraphicCamera(const Vec3d& pos, 
    const Vec3d& aim, const Vec3d up, float near, float far, 
    float ViewAngle) {
  this->GetCoordinate(pos, aim, up);

  near_ = near;
  far_  = far;
  fov_  = ViewAngle;

  Init();
}

void GraphicCamera::SetClippingPlanes(float near, float far) {
  near_ = near;
  far_  = far;
}

void GraphicCamera::SetCenterOfFocus(const Vec3d& new_center) {
  Vec3d dif = new_center - aim_;
  GetCoordinate(pos_ + dif, new_center, up_);
}

// Set up perspective display with this camera
void GraphicCamera::PerspectiveDisplay(int width, int height) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(fov_, (float) width/ (float) height, near_, far_);
  gluLookAt(pos_.x(), pos_.y(), pos_.z(),
            aim_.x(), aim_.y(), aim_.z(),
            up_.x(),  up_.y(),  up_.z());
}

void GraphicCamera::MouseEventHandler(int button, int state, int x, int y){ 
  double realy, wx, wy, wz;
  // if ALT key used
  int mode = glutGetModifiers();  
  
  if (state = GLUT_UP && CameraMode != kINACTIVE) {
    current_elev += dt_elev;
    current_azim += dt_azim;
    //reset change in elevation and roll of the camera;
    dt_elev = dt_azim = 0.0;
    CameraMode = kINACTIVE;
  } else if (state == GLUT_DOWN) { 
    MouseStartX = MousePrevX = x;
    MouseStartY = MousePrevY = y;
    switch(button) {
      case GLUT_LEFT_BUTTON:
        //rotating 
        CameraMode = kROTATE;
        break;
      case GLUT_MIDDLE_BUTTON:
        // translating
        CameraMode = kTRANSLATE;
        glGetIntegerv(GL_VIEWPORT, ViewPort);
        glGetDoublev(GL_MODELVIEW_MATRIX, MvMat.GetPtr());
        glGetDoublev(GL_PROJECTION_MATRIX, ProjMat.GetPtr());
        // height of window in pixels
        realy = ViewPort[3] - y - 1;
        gluProject(aim_.x(), aim_.y(), aim_.z(), MvMat.GetPtr(), 
                   ProjMat.GetPtr(), ViewPort, &wx, &wy, &wz);
        gluUnProject((GLdouble) x, (GLdouble) realy, wz, MvMat.GetPtr(),
                     ProjMat.GetPtr(), ViewPort, &PrevMousePos.x(), 
                     &PrevMousePos.y(), &PrevMousePos.z()); 
        break;
      case GLUT_RIGHT_BUTTON:
        CameraMode = kZOOM;
        break;
    }
  }
}

//
// Rotation routines - regular functions, not methods
//

void RotateX(Vec3d &v, double degree){
  double c = cos(Deg2Rad(degree));
  double s = sin(Deg2Rad(degree));
  double v1 = v[1] * c - v[2] * s;
  double v2 = v[1] * s + v[2] * c;
  v[1] = v1; v[2] = v2; 
  return;
}

void RotateY(Vec3d &v, double degree){
    double c = cos(Deg2Rad(degree));
      double s = sin(Deg2Rad(degree));
        double v0 = v[0] * c + v[2] * s;
          double v2 = -v[0] * s + v[2] * c;
            v[0] = v0; v[2] = v2; 
}

void ArbitraryRotate(Vec3d U, Vec3d V, Vec3d W,
                     double degreeX, double degreeY,
                     Vec3d& point, Vec3d aim) {
  double cx = cos(Deg2Rad(degreeX));
  double sx = sin(Deg2Rad(degreeX));
  double cy = cos(Deg2Rad(degreeY));
  double sy = sin(Deg2Rad(degreeY));
  
  Matrixd trans = { {1, 0, 0, -aim[0]},
    {0, 1, 0, -aim[1]},
    {0, 0, 1, -aim[2]},
    {0, 0, 0, 1}};
  
  Matrixd mat = { {U[0], U[1], U[2], 0},
    {V[0], V[1], V[2], 0},
    {W[0], W[1], W[2], 0},
    {0, 0, 0, 1}};
  
  Matrixd rot(4, 4);
  Matrixd pos =  {{point[0]}, {point[1]}, {point[2]}, {1}};
  
  pos = trans * pos;
  
  pos = mat*pos;
  
  rot = {{1,   0,  0, 0},
    {0,  cx, sx, 0},
    {0, -sx, cx, 0},
    {0,   0,  0, 1}};
  
  pos = rot*pos;
  
  rot = {{ cy, 0, sy, 0},
    {0, 1,  0, 0},
    {-sy, 0, cy, 0},
    {0, 0,  0, 1}};
  
  pos = rot * pos;
  
  pos = mat.inv()*pos;
  
  pos = trans.inv()*pos;
  
  point = {pos.get(0, 0), pos.get(1, 0), pos.get(2, 0)};
}


void GraphicCamera::MouseMotionEventHandler(int x, int y) {
  int mouse_dx, mouse_dy, d;
  double z;
  Vec3d mouse_pos, dir;
  Vec3d WndX, WndY, WndZ;
  float realy;
  double wx, wy, wz;

  if (CameraMode != kINACTIVE) {
    mouse_dx = x - MousePrevX;
    mouse_dy = y - MousePrevY;

    if (abs(mouse_dx) > abs(mouse_dy)) {
      d = mouse_dx;
    } else {
      d = mouse_dy;
    }

    switch(CameraMode) {

      case kZOOM:
        z = (double) d / 100.0;
        dir = aim_ - pos_;
        if (dir.Norm() < 0.1 && z > 0) {
          z *= 10.0;
          aim_ = aim_ + z * dir;
        }
        pos_ = pos_ + z * dir;
        break;
      case kROTATE:
        dt_azim = ((double) (x - MouseStartX)) / 5.0;
        dt_elev = ((double) (y - MouseStartY)) / 5.0;
        
        loc_dt_azim = ((double) mouse_dx) / 5.0;
        loc_dt_elev = ((double) mouse_dy) / 5.0;

        WndX = {1.0, 0.0, 0.0};
        WndY = {0.0, 1.0, 0.0};

        RotateX(WndX, current_elev + dt_elev);
        RotateY(WndY, current_azim + dt_azim);
        WndX.z() *= -1;
        
        RotateX(WndX, current_elev + dt_elev);
        RotateY(WndY, current_azim + dt_azim);
        WndX.z() *= -1;

        WndZ = (WndX % WndY).Normalization();

        ArbitraryRotate(WndX, WndY, WndZ, loc_dt_elev, 0, pos_, aim_);
        ArbitraryRotate(Vec3d(1, 0, 0), Vec3d(0, 1, 0), Vec3d(0, 0, 1), 0,
                        -loc_dt_azim, pos_, aim_);
        RotateX(WndX, current_azim + dt_elev);
        RotateY(WndY, current_azim + dt_azim);
        WndX.z() *= -1;
        up_ = WndY.Normalize();

        break;
      case kTRANSLATE:

      realy = ViewPort[3] - y - 1;
//      gluProject(aim_.x(), aim_.y(), aim.z(), MvMat, ProjMat, ViewPOrt,
//                 &wx, &wy, &wz);
//
//      gluUnProject((GLdouble) x, (GLdouble) realy, wz, MvMat, ProjMat, 
//                   ViewPort, &mouse_pos.x(), &mouse_pos.y(), 
//                   &mouse_pos.z());
      
      // move both the camera position and its aim coordinate
      dir = mouse_pos - PrevMousePos;
      pos_ = pos_ - dir;
      aim_ = aim_ - dir;

      PrevMousePos = mouse_pos;
      break;
    }
    MousePrevX = x;
    MousePrevY = y;
  }
}

const GraphicCamera& GraphicCamera::operator=(const GraphicCamera& cam) {
  aim_ = cam.aim_;
  pos_ = cam.pos_;
  up_ = cam.up_;

  return *this;
}
