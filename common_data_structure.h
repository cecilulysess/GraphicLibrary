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

namespace GL_CONTROL_DEF {
  const unsigned int KAA_MARKER = 0x1;
  const unsigned int KDOF_MARKER = 0x2;

  const unsigned int KRM_AAONLY = 0x0 ^ KAA_MARKER;
  const unsigned int KRM_DOF_AA = KAA_MARKER | KDOF_MARKER;
  const unsigned int KRM_DOF_ONLY = KDOF_MARKER;
}//ns GL_CONTROL_DEF
#endif
