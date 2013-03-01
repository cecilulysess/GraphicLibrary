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

// This is a Camera that mimic the common mouse interaction with screen
//   in 3D modeling program.
class GraphicCamera {
public:
  // Constructor with position, aim, and up vector
  GraphicCamera(const Vec3d& pos, const Vec3d& aim, const Vec3d up);

  // Constructor with position, aim, and up vercor as well as the
  //  clipping plane and view angle (FoV)
  GraphicCamera(const Vec3d& pos, const Vec3d& aim, const Vec3d up,
                float near, float far, float ViewAngle);
  // Set up OpenGL perspective matrix and MV matrix with this camera 
  void PerspectiveDisplay(int width, int height); 

  // set the clipping planes
  void SetClippingPlanes(float near, float far);
  // set the direction aim position
  void SetCenterOfFocus(const Vec3d& new_center);

  void MouseEventHandler(int button, int state, int x, int y);
  void MouseMotionEventHandler(int x, int y);

  const Camera& operator=(const Camera& cam);

  const Vec3d& position() const { return position_; } 
  const Vec3d& aim() const { return aim_; } 
  const Vec3d& up() const { return up_; } 
  float fov() const {return fov_; }
  float near() const {return near_;}
  float far() const {return far_;}
  float focus() const {return focus_;}

  Vec3d& position() { return position_; } 
  Vec3d& aim() { return aim_; } 
  Vec3d& up() { return up_; } 
  float& near() {return near_;}
  float& far() {return far_;}
  float& fov() {return fov_; }
  float& focus() {return focus_;}
  
private:
  void GetCoordinate(const Vec3d& pos, const Vec3d& aim, const Vec3d& up);
  void Init();
  Vec3d default_position;
  Vec3d default_aim;
  Vec3d default_up;
 
  double default_azim;
  double default_elev;
  double current_azim;
  double current_elev;

  Vec3d pos_, aim_, up_;
  float near_, far_, fov_, focus_;  

}

#endif // __Graphic__GraphicCamera__
