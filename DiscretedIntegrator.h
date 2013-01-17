//
//  DiscretedIntegrator.h
//  Graphic
//
//  Created by Julian Wu on 1/17/13.
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//

#ifndef __Graphic__DiscretedIntegrator__
#define __Graphic__DiscretedIntegrator__

#include"common_data_structure.h"

class DiscretedIntegrator{
public:
  // Integrate f(x)*g(x) with it's upper and lower bound
  double Integrate(point *fx, point *gx, int size) ;
};
#endif /* defined(__Graphic__DiscretedIntegrator__) */
