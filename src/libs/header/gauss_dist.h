#ifndef GAUSS_DIST_H_
#define GAUSS_DIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double r1_dist(double realdist, double pfl, double foundpfl);
double r7_dist(double realdist2, double pfl2, double foundpfl2);
double r4_dist(int x1, int y1, int x2, int y2,double pfl);
double r8_dist(double r7, double r1);
double r2_dist(double r8, double r4);
double r3_dist(double r7, double r4);
double biggear_angle(double realdist, double pfl, double foundpfl, double realdist2, double foundpfl2, int x1, int y1, int x2, int y2);
double smallgear_angle(double r2, int y1, int y2, double pfl_2);


#endif