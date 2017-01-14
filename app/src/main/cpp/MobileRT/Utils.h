//
// Created by Tiago on 14-11-2016.
//

#ifndef MOBILERAYTRACER_UTILS
#define MOBILERAYTRACER_UTILS

#include <utility>

float fastArcTan(const float value);

double haltonSequence(const int index, const int base);
unsigned int roundToPower2(unsigned int v);

#endif //MOBILERAYTRACER_UTILS