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

namespace GraphicCamera {
  
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
void GraphicCamera::GetCoordinate(const Vec3d& pos, const Vec3d& aim,
                                  const Vec3d& up){
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

GraphicCamera::GraphicCamera() :
                             fru_(60, 1.333,0.02, 100){
  pos_ = {0.0, 1.0, 2.0};
  aim_ = {0.0, 0.0, 0.0};
  up_  = {0.0, 1.0, 0.0};

  this->focus_ = 1;
  Init();
}

// Constructor with position, aim, and up vector
GraphicCamera::GraphicCamera(const Vec3d& pos, 
                             const Vec3d& aim, const Vec3d up, double focus) :
                             fru_(60, 1.333, 0.02, 100){
  this->GetCoordinate(pos, aim, up);
  this->focus_ = focus;

  Init();
}

// Constructor with position, aim, and up vercor as well as the
//  clipping plane and view angle (FoV)
GraphicCamera::GraphicCamera::GraphicCamera(const Vec3d& pos, 
    const Vec3d& aim, const Vec3d up, float near, float far, 
                                            float ViewAngle,
                                            double focus) :
    fru_(ViewAngle, 1.333, near, far){
  this->GetCoordinate(pos, aim, up);
  this->focus_ = focus;
  Init();
}

void GraphicCamera::SetClippingPlanes(float near, float far) {
  fru_.Near() = near;
  fru_.Far() = far;
}

void GraphicCamera::SetCenterOfFocus(const Vec3d& new_center) {
  Vec3d dif = new_center - aim_;
  GetCoordinate(pos_ + dif, new_center, up_);
}

// Set up perspective display with this camera
void GraphicCamera::PerspectiveDisplay(int width, int height) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  gluPerspective(fru_.FoV(), (float) width/ (float) height,
                 fru_.Near(), fru_.Far());
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(pos_.x(), pos_.y(), pos_.z(),
            aim_.x(), aim_.y(), aim_.z(),
            up_.x(),  up_.y(),  up_.z());
//  glMatrixMode(GL_PROJECTION);
//  glLoadIdentity();
//  frustum = new Frustum(60, (float)WIDTH / (float)HEIGHT, 0.02, 20.0);
//  glLoadMatrixd(frustum->GetMatrix().Transpose().GetPtr());
//  glMatrixMode(GL_MODELVIEW);
//  glLoadIdentity();
//  glRotated(0, 0, 0, 1);
//  glRotated(10, 1, 0, 0);
//  glRotated(0, 0, 1, 0);
//  glTranslated(0, -0.15, -0.4);
}

void GraphicCamera::MouseEventHandler(int button, int state, int x, int y){ 
  double realy, wx, wy, wz;
  // if ALT key used
//  int mode = glutGetModifiers();  
  
  if (state == GLUT_UP && CameraMode != kINACTIVE) {
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
//        RotateX(WndX, current_azim + dt_elev);
//        RotateY(WndY, current_azim + dt_azim);
        WndX.z() *= -1;
        up_ = WndY.Normalization();

        break;
      case kTRANSLATE:

      realy = ViewPort[3] - y - 1;
      gluProject(aim_.x(), aim_.y(), aim_.z(), MvMat.GetPtr(),
                 ProjMat.GetPtr(), ViewPort,
                 &wx, &wy, &wz);

      gluUnProject((GLdouble) x, (GLdouble) realy, wz, MvMat.GetPtr(),
                   ProjMat.GetPtr(),
                   ViewPort, &mouse_pos.x(), &mouse_pos.y(), 
                   &mouse_pos.z());
      
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

  

  

//=====================Below is the implementation of AA and DoF===============
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

static inline Matrixd& GetJitterTable(int level) {
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
  Matrixd& jitterTable = GetJitterTable(level);
  glClearAccum(0, 0, 0, 0);
  glClear(GL_ACCUM_BUFFER_BIT);
  for (int i = 0 ; i < level; ++i) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    JitterCamera(jitterTable.get(i, 0) * time_factor,
                 jitterTable.get(i, 1) * time_factor);
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
    //  glFlush();
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
  //  dx = (eye_x - fru->Near()/focus * eye_x);
  dy = - (pix_y * frustumHeight/WndHeight + eye_y * fru_.Near() / focus);
  //  printf("world delta = %f %f\n",dx,dy);q
  glFrustum(fru_.Left() + dx, fru_.Right() + dx, fru_.Bottom() + dy,
            fru_.Top() + dy, fru_.Near(), fru_.Far() );
  glMatrixMode(GL_MODELVIEW);
//  //  glPushMatrix();
//  //Problematic to do a view translation here, it will remove the materials.
//  glLoadIdentity();
//  glRotated(0, 0, 0, 1);
//  glRotated(10, 1, 0, 0);
//  glRotated(0, 0, 1, 0);
//  glTranslated(0, -0.15, -0.4);
//  //  cout<<"Translate Eye:"<<eye_x <<", "<<eye_y<<endl;
//  glTranslatef(-eye_x, -eye_y, 0.0);
//  //  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(pos_.x(), pos_.y(), pos_.z(),
            aim_.x(), aim_.y(), aim_.z(),
            up_.x(),  up_.y(),  up_.z());
  glTranslated(-eye_x, -eye_x, 0.0);

//  glTranslated(-pos_.x(), -pos_.y(), -pos_.z());
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
  
  
  inline double& Frustum::Left() {
    return this->left;
  }
  
  inline double& Frustum::Right() {
    return this->right;
  }
  
  
  inline double& Frustum::Top(){
    return this->top;
  }
  
  inline double& Frustum::Bottom(){
    return this->bottom;
  }
  
  inline double& Frustum::Near(){
    return this->near;
  }
  
  inline double& Frustum::Far(){
    return this->far;
  }
  
  inline double& Frustum::FoV() {
    return  this->fov;
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
 
} //ns GraphicCamera