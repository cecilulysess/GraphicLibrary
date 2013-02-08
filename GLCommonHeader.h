//
//  GLCommonHeader.h
//  Graphic
//  The common header file which loading necessary OpenGL Include files
//
//  Created by Julian Wu on 1/31/13.
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//

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
