//
//  GraphicMath.cpp
//  Graphic
//
//  Created by Julian Wu on 2/5/13.
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//

#include "GraphicMath.h"

#include <iostream>
#include <vector>
#include <math.h>


Vec3d::Vec3d(std::initializer_list<double> il) : Vector<double>(il){
  if (il.size() != 3) {
    throw "Illigal parameter length";
  }
}

Vec3d::Vec3d(Vec3d& li) : Vector<double>(li) {
  if (li.size() != 3) {
    throw "Illigal parameter length";
  }
}

Vec3d& Vec3d::Normalize(){
  Vec3d* res = new Vec3d(*this);
  *res *= (1/Norm());
  return *res;
}

double Vec3d::Norm(){
  return sqrtl(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}