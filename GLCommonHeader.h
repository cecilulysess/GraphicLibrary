//
//  GLCommonHeader.h
//  Graphic
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//
//  Author: cosmobserver@gmail.com (Yanxiang Wu <Julian>)
//
//  The common header file which loading necessary OpenGL Include files
//
//  Created on 1/31/13

#ifndef Graphic_GLCommonHeader_h
#define Graphic_GLCommonHeader_h

#ifdef __APPLE__
  #include<GLUT/glut.h>
  #include<OpenGL/gl.h>
  #include<OpenGL/glu.h>
  #include<OpenGL/glext.h>
#else
  #define GL_GLEXT_PROTOTYPES 1
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include <GL/glut.h>
  #include <GL/glx.h>
  #include <GL/glext.h>
#endif


#endif
