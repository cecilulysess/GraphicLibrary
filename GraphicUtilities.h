//
//  GraphicUtilities.h
//  Graphic
//
//  Created by Julian Wu on 1/31/13.
//  Copyright (c) 2013 Julian Wu. All rights reserved.
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


#endif /* defined(__Graphic__GraphicUtilities__) */
