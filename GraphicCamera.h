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

#ifndef __Graphic__GraphicCamera__
#define __Graphic__GraphicCamera__

#include <math.h>
#include <stdio.h>

typedef void (*render_callback)();

namespace GraphicCamera {
  
typedef struct vec3d{ 
  float x, y, z;
  vec3d(float x, float y, float z) { this->x = x, this->y = y, this->z = z;}
  vec3d(const float* v) { x = v[0], y = v[1], z = v[2];}
  vec3d() {x=0, y=0, z=0;}
  vec3d(vec3d& b) { x = b.x, y = b.y, z = b.z; }
  float Norm() { return sqrt(x*x + y*y + z*z); }
  vec3d& Normalization() { return *(new vec3d(x/Norm(), y/Norm(), z/Norm())); }
  vec3d& cross(const vec3d& b) const { 
    float a1 = x, a2 = y, a3 = z, b1 = b.x, b2 = b.y, b3 = b.z;
    return *(new vec3d(a2 * b3 - a3 * b2, -(a1 * b3 - a3 * b1), a1 * b2 - a2 * b1));
  }
  void print_v(){fprintf(stderr, "\tVec: %f %f %f\n", x, y, z);}
} vec3d;

class Frustum {
public:
  
  Frustum(float left, float right, float bottom, float top,
          float near, float far);
  
  Frustum(float fovy, float aspect, float near, float far);
  
  // Draw current frustum, need alittle changes in detail
  void DrawFrustum(float fovY, float aspectRatio, float nearPlane, float farPlane);
  
  double& Left() ;
  double& Right() ;
  double& Top();
  double& Bottom();
  double& Near();
  double& Far();
  double& FoV();
  double* GetMatrix();
  
  
private:
  double near, far, left, right, bottom, top, fov;
  
  double matrix[16];
};


// This is a Camera that mimic the common mouse interaction with screen
//   in 3D modeling program.
class GraphicCamera {
public:
  // GraphicCamera();
  // Constructor with position, aim, and up vector
  GraphicCamera(const float* pos, const float* aim, const float* up,
                double focus);

  // // Constructor with position, aim, and up vercor as well as the
  // //  clipping plane and view angle (FoV)
  // GraphicCamera(const vec3d& pos, const vec3d& aim, const vec3d up,
  //               float near, float far, float ViewAngle,
  //               double focus);
  // Set up OpenGL perspective matrix and MV matrix with this camera 
  void PerspectiveDisplay(int width, int height);
  
  void AAPerspectiveDisplay(int width, int height, int level,
                            render_callback render_frame);
  void DoFPerspectiveDisplay(int width, int height, int blur_level,
                             render_callback render_frame);


  const GraphicCamera& operator=(const GraphicCamera& cam);

  const vec3d& position() const { return pos_; } 
  const vec3d& aim() const { return aim_; } 
  const vec3d& up() const { return up_; } 
  float focus() const {return focus_;}

  vec3d& position() { return pos_; } 
  vec3d& aim() { return aim_; } 
  vec3d& up() { return up_; }
  const Frustum& frustum() {return fru_;}
  float& focus() {return focus_;}
  
private:
  void JitterCamera(GLfloat pix_x, GLfloat pix_y);
  void JitterCamera(GLfloat pix_x, GLfloat pix_y,
               GLfloat eye_x, GLfloat eye_y,
               GLdouble focus);
  void GetCoordinate(const vec3d& pos, const vec3d& aim, const vec3d& up);
  
  vec3d default_position;
  vec3d default_aim;
  vec3d default_up;
 
  double default_azim;
  double default_elev;
  double current_azim;
  double current_elev;

  vec3d pos_, aim_, up_;
  float focus_;
  Frustum fru_;

};
} // ns GraphicCamera
#endif // __Graphic__GraphicCamera__
