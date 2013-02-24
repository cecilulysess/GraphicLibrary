//
//  GLCommonHeader.h
//  Graphic
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//
//  Released under Apache 2.0 listence
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

#define MAIN_PROG
//#define TESTING_

static const char* VERT_SHADER_FILE_DIR =
 // "/Users/julian/Programming/CS605/Graphic/Graphic/el.vert";
  "/home/yanxiaw/Programming/cs605/GraphicLibrary/el.vert";

static const char* PHONG_FRAG_SHADER_FILE_DIR =
 // "/Users/julian/Programming/CS605/Graphic/Graphic/el.frag";
  "/home/yanxiaw/Programming/cs605/GraphicLibrary/el.frag";

static const char* BLINN_PHONG_FRAG_SHADER_FILE_DIR = 
  "/home/yanxiaw/Programming/cs605/GraphicLibrary/bp.frag";
#endif
