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

#include "GraphicMath.h"


typedef void (*render_callback)();
namespace GraphicCamera {
  

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
  Matrix4d& GetMatrix();
  
  
private:
  double near, far, left, right, bottom, top, fov;
  
  Matrix4d matrix;
};


// This is a Camera that mimic the common mouse interaction with screen
//   in 3D modeling program.
class GraphicCamera {
public:
  GraphicCamera();
  // Constructor with position, aim, and up vector
  GraphicCamera(const Vec3d& pos, const Vec3d& aim, const Vec3d up,
                double focus);

  // Constructor with position, aim, and up vercor as well as the
  //  clipping plane and view angle (FoV)
  GraphicCamera(const Vec3d& pos, const Vec3d& aim, const Vec3d up,
                float near, float far, float ViewAngle,
                double focus);
  // Set up OpenGL perspective matrix and MV matrix with this camera 
  void PerspectiveDisplay(int width, int height);
  
  void AAPerspectiveDisplay(int width, int height, int level,
                            render_callback render_frame);
  void DoFPerspectiveDisplay(int width, int height, int blur_level,
                             render_callback render_frame);
  
  // set the clipping planes
  void SetClippingPlanes(float near, float far);
  // set the direction aim position
  void SetCenterOfFocus(const Vec3d& new_center);

  void MouseEventHandler(int button, int state, int x, int y);
  void MouseMotionEventHandler(int x, int y);

  const GraphicCamera& operator=(const GraphicCamera& cam);

  const Vec3d& position() const { return pos_; } 
  const Vec3d& aim() const { return aim_; } 
  const Vec3d& up() const { return up_; } 
  float focus() const {return focus_;}

  Vec3d& position() { return pos_; } 
  Vec3d& aim() { return aim_; } 
  Vec3d& up() { return up_; }
  const Frustum& frustum() {return fru_;}
  float& focus() {return focus_;}
  
private:
  void JitterCamera(GLfloat pix_x, GLfloat pix_y);
  void JitterCamera(GLfloat pix_x, GLfloat pix_y,
               GLfloat eye_x, GLfloat eye_y,
               GLdouble focus);
  void GetCoordinate(const Vec3d& pos, const Vec3d& aim, const Vec3d& up);
  void Init();
  
  Vec3d default_position;
  Vec3d default_aim;
  Vec3d default_up;
 
  double default_azim = 0.0;
  double default_elev = 0.0;
  double current_azim = 0.0;
  double current_elev = 0.0;

  Vec3d pos_, aim_, up_;
  float focus_;
  Frustum fru_;

};
} // ns GraphicCamera
#endif // __Graphic__GraphicCamera__
