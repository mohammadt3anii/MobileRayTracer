//
// Created by Tiago on 14-11-2016.
//

#include "Utils.h"
#include "Constants.h"
#include <cmath>

//http://nghiaho.com/?p=997
float fastArcTan(const float value) {
    const float abs((value < 0) ? -value : value);
    return PI_4 * value - (value * (abs - 1)) * (0.2447f + (0.0663f * abs));
}

float haltonSequence(const int index, const int base) {
    float result = 0;
    float f = 1;
    float i = index;
    while (i > 0) {
        f = f / base;
        result = result + f * (static_cast<int> (i) % base);
        i = std::floor(i / base);
    }
    return result;
}