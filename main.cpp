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

int main(int argc, const char * argv[])
{
  DiscretedIntegrator integrator;
  double res = integrator.Integrate(fx, rlambda, 41);
  cout<<res<<endl;
  return 0;
}

