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

namespace GraphicUtilities{
  
typedef void (*render_callback)();



class GraphicUtilities {
public:
  static char* read_shader_program(const char *filename);

  // load a texture into specific texture id
  static bool LoadTexture(char *filename, GLuint &texture);
};

// set of functions as short cut of OpenGL utility function
class GLShortCut{
public:
  static void PrintGLErrors(char *file, int line); 
};

}// ns GraphicUtilities

#endif /* defined(__Graphic__GraphicUtilities__) */
