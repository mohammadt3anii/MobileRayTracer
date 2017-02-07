//
// Created by Tiago on 14-11-2016.
//

#ifndef MOBILERAYTRACER_UTILS
#define MOBILERAYTRACER_UTILS

#include <cmath>

#define RAY_LENGTH_MIN  1.0e-5f
#define RAY_LENGTH_MAX  1.0e+10f
#define PI              3.14159265358979323846f
#define PI_4            0.78539816339744830962f
#define VECT_PROJ_MIN   1.0e-8f // kEpsilon

float fastArcTan(const float value);

double haltonSequence(const int index, const int base);

unsigned int roundToPower2(unsigned int v);

#endif //MOBILERAYTRACER_UTILS