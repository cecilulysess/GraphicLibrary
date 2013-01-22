//
//  main.cpp
  //  Graphic
//
//  Created by Julian Wu on 1/17/13.
//  Copyright (c) 2013 Julian Wu. All rights reserved.
//

#include <iostream>
#include"common_data_structure.h"
#include"DiscretedIntegrator.h"
#include"CIE_matching_functions.h"
#include"matrix.h"

using std::cout;
using std::endl;

struct point rlambda[] = {
  380.0, 0.25,
	390.0, 0.35,
	400.0, 0.50,
	410.0, 0.60,
	420.0, 0.65,
	430.0, 0.71,
	440.0, 0.71,
	450.0, 0.72,
	460.0, 0.72,
	470.0, 0.70,
	480.0, 0.65,
	490.0, 0.60,
	500.0, 0.54,
	510.0, 0.50,
	520.0, 0.40,
	530.0, 0.30,
	540.0, 0.20,
	550.0, 0.12,
	560.0, 0.08,
	570.0, 0.04,
	580.0, 0.04,
	590.0, 0.05,
	600.0, 0.04,
	610.0, 0.04,
	620.0, 0.05,
	630.0, 0.08,
	640.0, 0.14,
	650.0, 0.14,
	660.0, 0.24,
	670.0, 0.14,
	680.0, 0.08,
	690.0, 0.05,
	700.0, 0.04,
	710.0, 0.04,
	720.0, 0.10,
	730.0, 0.20,
	740.0, 0.30,
	750.0, 0.40,
	760.0, 0.50,
	770.0, 0.60,
};
struct point px[] = {
  380.0, 1.0,
	390.0, 1.0,
	400.0, 1.0,
	410.0, 1.0,
	420.0, 1.0,
	430.0, 1.0,
	440.0, 1.0,
	450.0, 1.0,
	460.0, 1.0,
	470.0, 1.0,
	480.0, 1.0,
	490.0, 1.0,
	500.0, 1.0,
	510.0, 1.0,
	520.0, 1.0,
	530.0, 1.0,
	540.0, 1.0,
	550.0, 1.0,
	560.0, 1.0,
	570.0, 1.0,
	580.0, 1.0,
	590.0, 1.0,
	600.0, 1.0,
	610.0, 1.0,
	620.0, 1.0,
	630.0, 1.0,
	640.0, 1.0,
	650.0, 1.0,
	660.0, 1.0,
	670.0, 1.0,
	680.0, 1.0,
	690.0, 1.0,
	700.0, 1.0,
	710.0, 1.0,
	720.0, 1.0,
	730.0, 1.0,
	740.0, 1.0,
	750.0, 1.0,
	760.0, 1.0,
	770.0, 1.0,
};

//matrix = {
//  0.69, 0.205, 0.15,
//  0.3 , 0.715, 0.045,
//  0.01, 0.080, 0.805
//}

int main(int argc, const char * argv[])
{
  DiscretedIntegrator integrator;
  double N = integrator.Integrate(fy, px, 41);
  double Xb = 1.0 / N * integrator.Integrate(fx, rlambda, 41);
  double Yb = 1.0 / N * integrator.Integrate(fy, rlambda, 41);
  double Zb = 1.0 / N * integrator.Integrate(fz, rlambda, 41);
  double T = Xb + Yb + Zb;
  double xb = Xb/T, yb = Yb/T, zb = Zb/T;
//  cout<<"N:"<<N<<",\t	Xb:"<<Xb<<",	Yb:"<<Yb<<",	Zb:"<<Zb<<endl;
//  cout<<"\t\t\t\txb:"<<xb<<",	yb:"<<yb<<",	zb:"<<zb<<endl;
  double white[3] = { 0.31271 / 0.32902, 1.0, (1.0 - 0.31271 - 0.32902) / 0.32902 };
  Matrix<double> test(3,3);
  Matrix<double> XYZw(3, 1, white);
  cout<<"XYZw:"<<endl;
  XYZw.show_matrix();
  double XYZrgb[9] = {
    0.69, 0.205, 0.15,
    0.30, 0.715, 0.045,
    0.01, 0.080, 0.805
  };
  
  Matrix<double> XYZRGB(3,3, XYZrgb);
//  XYZRGB.show_matrix();
//  XYZRGB.inv().show_matrix();
  Matrix<double> CRGB = XYZRGB.inv() * XYZw;
  cout<<"Crgb:"<<endl;
  CRGB.show_matrix();
  double CRGBmat[9] = {
    CRGB.get(0, 0), CRGB.get(1, 0), CRGB.get(2, 0),
    CRGB.get(0, 0), CRGB.get(1, 0), CRGB.get(2, 0),
    CRGB.get(0, 0), CRGB.get(1, 0), CRGB.get(2, 0),
  };
  Matrix<double> CRGBMAT(3,3, CRGBmat);
  double XYZ_raw[3] = { Xb, Yb, Zb };
  Matrix<double> XYZ(3, 1, XYZ_raw);
  cout<<"XYZ:"<<endl;
  XYZ.show_matrix();
  Matrix<double> RGB = (CRGBMAT.elementwise_product(XYZRGB)).inv() * XYZ;
  cout<<"Final Result:"<<endl;
  RGB.show_matrix();
  
//  double testmatrix[12] = {1, 3, 6,
//                      2, 8, 10,
//    20, 60, 80,};
////                      110, 220, 330};
//  Matrix<double> mat(3, 3, testmatrix);
//  mat.show_matrix();
//  mat.inv().show_matrix();
////  mat *= XYZw;
////  mat.show_matrix();
////  
////  Vector<double> vv = mat.get_row(2);
////  Vector<double> va = mat.get_row(1);
////  
////  
////  cout<< va *  vv<<endl;
  return 0;
}

