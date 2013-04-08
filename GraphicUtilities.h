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

#include "GraphicMath.h"

namespace GraphicUtilities{
  
typedef void (*render_callback)();
class Frustum {
public:
  
  Frustum(float left, float right, float bottom, float top,
          float near, float far);
  
  Frustum(float fovy, float aspect, float near, float far);
  
  // Draw current frustum, need alittle changes in detail
  void DrawFrustum(float fovY, float aspectRatio, float nearPlane, float farPlane);
  
  double Left() ;
  double Right() ;
  double Top();
  double Bottom();
  double Near();
  double Far();
  Matrix4d& GetMatrix();

    
private:
  float near, far, left, right, bottom, top;
  
  Matrix4d matrix;
};


class GraphicUtilities {
public:
  // AntiAlias the image, require glutInitDisplayMode(GLUT_ACCUM)
  static void AntiAlias(int level, render_callback render_frame, Frustum* fru);
  
  // Depth of Field effect
  static void DoFScene(render_callback render_frame, Frustum* fru,
                       double focus, int blur_level);
  
  static char* read_shader_program(const char *filename);
  
  static void JitterCamera(GLfloat pix_x, GLfloat pix_y, Frustum* fru);
  static void JitterCamera(GLfloat pix_x, GLfloat pix_y,
                                      GLfloat eye_x, GLfloat eye_y,
                                      GLdouble focus,
                                      Frustum* fru);

  static void DrawGrid(float size, float step);
};

// set of functions as short cut of OpenGL utility function
class GLShortCut{
  public:
  static void PrintGLErrors(char *file, int line); 
};
/*class Camera {
public:
  float near_plane();
  float far_plane();
  float fov();
  
private:
  float _near_plane;
  float _far_plane;
  float _fov;
  
};*/
}// ns GraphicUtilities

#endif /* defined(__Graphic__GraphicUtilities__) */
