//
//  DiscretedIntegrator.cpp
//  Graphic
//
//  Created by Julian Wu on 1/17/13.
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//

#include "DiscretedIntegrator.h"

#include <stdio.h>
#include <math.h>

// get h(x) = ax + b given 2 point in h(x) xa, xb
inline void getAB(point xa, point xb, double* a, double* b) {
  double tmp = 0.0;
  tmp = (xb.y - xa.y) / (xb.x - xa.x);
  *a = tmp;
  *b = - xa.x * tmp + xa.y;
}

// intergeted one
inline double Hx(double a, double b, double c, double d, double X) {
  return (a * c) / 3 * pow(X, 3) + (b * c + a * d) / 2 * pow(X, 2) + b * d * X;
}

inline double INTFxGx(double a, double b, double c, double d, double up, double lo){
  return Hx(a, b, c, d, up) - Hx(a, b, c, d, lo);
}

double DiscretedIntegrator::Integrate(point *fx, point *gx, int size) {
  double subsum  = 0.0, sum = 0.0;
  double fa, fb, ga, gb;
  for (int i = 0; i < size - 2; ++i) {
    subsum = 0.0;
    point xa = fx[i],
          xb = gx[i],
          ya = fx[i+1],
          yb = gx[i+1];
    getAB(xa, ya, &fa, &fb);
    getAB(xb, yb, &ga, &gb);
//    printf("x: %f, fa: %f, fb: %f, ga: %f, gb: %f ", xa.x, fa, fb, ga, gb);
    subsum = INTFxGx(fa, fb, ga, gb, ya.x, xa.x);
//    printf("Subsum:%f\n", subsum);
    sum += subsum;
  }
  return sum;
}