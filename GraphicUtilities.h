//
//  GraphicUtilities.h
//  Graphic
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//
//  Author: cosmobserver@gmail.com (Yanxiang Wu <Julian>)
//
//  This file contains very useful self defined utilities that would be
//    very helpful to use when develop openGL program.
//
//  Created on 1/31/13.
//

#ifndef __Graphic__GraphicUtilities__
#define __Graphic__GraphicUtilities__

typedef void (*render_callback)();
class Frustum {
public:
  
  Frustum(float near, float far, float left, float right, float bottom, float top);
  
  Frustum(float fovy, float aspect, float near, float far);
  
  // Draw current frustum, need alittle changes in detail
  void DrawFrustum(float fovY, float aspectRatio, float nearPlane, float farPlane);
  
  double Left() {
    return this->left;
  }
  
  double Right() {
    return this->right;
  }
  
  double Top(){
    return this->top;
  }
  
  double Bottom(){
    return this->bottom;
  }
  
  double Near(){
    return this->near;
  }
  
  double Far(){
    return this->far;
  }
  
  const double* GetMatrix(){
    return this->matrix;
  }
  
private:
  float near, far, left, right, bottom, top;
  
  double matrix[16];
};


class GraphicUtilities {
public:
  // AntiAlias the image, require glutInitDisplayMode(GLUT_ACCUM)
  static void AntiAlias(int level, render_callback render_frame, Frustum* fru);
  
  static char* read_shader_program(char *filename);
  
  static void JitterCamera(GLfloat pix_x, GLfloat pix_y, Frustum* fru);

  static void DrawGrid(float size, float step);
};

class Camera {
public:
  float near_plane();
  float far_plane();
  float fov();
  
private:
  float _near_plane;
  float _far_plane;
  float _fov;
  
  
  
};


#endif /* defined(__Graphic__GraphicUtilities__) */
