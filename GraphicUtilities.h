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

class GraphicUtilities {
public:
  static void AntiAlias(int level);
  
  static char* read_shader_program(char *filename);
  

};

#endif /* defined(__Graphic__GraphicUtilities__) */
