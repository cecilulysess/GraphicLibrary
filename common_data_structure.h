//
//  common_data_structure.h
//  Graphic
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//
//  Author: cosmobserver@gmail.com (Yanxiang Wu <Julian>)
//
//  Created on 1/17/13

#ifndef Graphic_common_data_structure_h
#define Graphic_common_data_structure_h

#include "GraphicMath.h"

//A alias for 3d point
//typedef Vec3d Point;
//typedef Vec3d Normal;

namespace GL_CONTROL_DEF {
  const unsigned int KAA_MARKER = 0x1;
  const unsigned int KDOF_MARKER = 0x2;

  const unsigned int KRM_AAONLY = 0x0 ^ KAA_MARKER;
  const unsigned int KRM_DOF_AA = KAA_MARKER | KDOF_MARKER;
  const unsigned int KRM_DOF_ONLY = KDOF_MARKER;
}//ns GL_CONTROL_DEF

namespace GLL_OBJECTS {
  // an abstract class that contains the frameworks for loading objects
  class GraphicObject {
    // a callback for draw itself, may be useful
    virtual void Draw() = 0;
    
  };
}
#endif
