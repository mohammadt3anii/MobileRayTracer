//
// Created by Tiago on 14-11-2016.
//

#include "Utils.h"
#include "Constants.h"
#include <cmath>

//http://nghiaho.com/?p=997
float fastArcTan(const float value) {
    const float abs((value < 0) ? -value : value);
    return PI_4 * value - (value * (abs - 1.0f)) * (0.2447f + (0.0663f * abs));
}

//https://en.wikipedia.org/wiki/Halton_sequence
float haltonSequence(const int index, const int base) {
    float f = 1.0f;
    float result = 0.0f;
    float i = index;
    while (i > 0.0f) {
        f = f / base;
        result = result + f * (static_cast<int> (i) % base);
        i = std::floor(i / base);
    }
    return result;
}

std::pair<float, float> limitsHaltonSequence(const unsigned int number) {
    float min = 1.0f;
    float max = 0.0f;
    for (unsigned int i(1); i <= number; i++) {
        const float temp(haltonSequence(i, 2));
        min = temp < min ? temp : min;
        max = temp > max ? temp : max;
    }
    return std::make_pair(min, max);
}