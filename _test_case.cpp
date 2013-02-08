//
//  test_case.cpp
//  Graphic
//
//  Created by Julian Wu on 2/7/13.
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//

#include "_test_case.h"

#include "GraphicMath.h"
#include <assert.h>
#include <math.h>
#include <time.h>


bool test_Vector() {
  clock_t ss = clock();
  Vector<double> zero = { 0.0, 0.0, 0.0 };
  Vector<double> a = { 1.99, 2.02, 6.7 };
  Vector<double> b = { -0.1, 7.0, 2.22 };
  Vector<double> c = { 3.3, -3.3, 0.0 };
  Vector<double> cmpa = { 1.990, 2.02000, 6.700000};
  
  // answer
  Vector<double> a_mult_b = {-0.1990,  14.1400,  14.8740};
  Vector<double> a_mult_c = {6.5670, -6.6660,     0};
  Vector<double> b_mult_c = {-0.3300,  -23.1000,        0};
  
  double a_dot_b = 28.815;
  double a_dot_c = -0.099;
  double b_dot_c = -23.43;
  
  Vector<double> a_dot2_99 = {5.9501,  6.0398, 20.0330};
  
  Vector<double> a_cro_b = {-42.4156, -5.0878, 14.1320};
  Vector<double> b_cro_a = {42.4156, 5.0878, -14.1320};
  Vector<double> a_cro_c = {22.1100, 22.1100, -13.2330};
  Vector<double> c_cro_a = {-22.1100, -22.1100, 13.2330};
  Vector<double> b_cro_c = {7.3260, 7.3260, -22.7700};
  Vector<double> c_cro_b = {-7.3260, -7.3260, 22.7700};
  
  Vector<double> na = {0.2735, 0.2777, 0.9209};
  Vector<double> nb = {-0.0136, 0.9531, 0.3023};
  Vector<double> nc = {0.7071, -0.7071,  0};
  
  
  assert(fabs(a[0] - 1.99) < 0.0000001);
//  std::cout<<fabs(a[2] - 2.02) <<std::endl;
  assert(fabs(a[1] - 2.02) < 0.0000001);
  assert(fabs(a[2] - 6.7) < 0.0000001);
  // C++ STL bug
//  double sa[] = {6.7*2.22};
//  double sb[] = {14.8740};
//  vector<double> va(sa, sa + sizeof(double));
//  vector<double> vb(sb, sb + sizeof(double));
//  assert(va == vb);
  assert(a == a && b == b && c == c);
  assert(a == cmpa);
//  std::cout<<a.ele_mult(b)[2] - a_mult_b[2]<<std::endl;;
  assert(a.ele_mult(b) == a_mult_b);
  assert(a[1] - 2.02 < 1e-9);
  assert(a.ele_mult(c) == a_mult_c);
  assert(b.ele_mult(c) == b_mult_c);
  assert(a * b - a_dot_b < 1e-9);
  assert(a * c - a_dot_c < 1e-9);
  assert(b * c - b_dot_c < 1e-9);
  assert(a * 2.99 == a_dot2_99);
  assert(a * 0.0 == zero );
  
  
  std::cout<<"Vector passed Test!"<<std::endl;
  
  
  
  Vec3d v1 = {1.99, 2.02, 6.7 };
  Vec3d v2 = {-0.1, 7.0, 2.22};
  Vec3d nv1 = {0.273526921854589,  0.277650443289583, 0.920919787148616};
  Vec3d nv2 = {-0.013616044753350, 0.953123132734531,  0.302276193524380};
  double n2 = 7.344276683241175;
  double n1 = 7.2753350;
//  std::cout<<v1.Norm() - n1<<std::endl;
  assert(v1.Norm() - n1 < 1e-6);
  assert(v2.Norm() - n2 < 1e-6);
  assert(v1.Normalize() == nv1);
  assert(v2.Normalize() == nv2);
  
  std::cout<<"Vec3d passed Test!"<<std::endl;
  
  clock_t  ed = clock();
  std::cout<<"Vector Test Running time: "<<
    (double) (ed - ss) / (double) CLOCKS_PER_SEC <<"s"<<std::endl;
  return true;
}

bool test_Matrix() {
  Matrixd mat = {{1, 3, 4}, {3, 3, 3}};
  
  
  return true;
}