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
double haltonSequence(const int index, const int base) {
    double f = 1.0;
    double result = 0.0;
    double i = index;
    while (i > 0.0) {
        f = f / base;
        result = result + f * (static_cast<int> (i) % base);
        i = std::floor(i / base);
    }
    return result;
}

std::pair<float, float> limitsHaltonSequence(const unsigned int number) {
    double min = 1.0;
    double max = 0.0;
    for (unsigned int i(1); i <= number; i++) {
        const double temp(haltonSequence(i, 2));
        min = temp < min ? temp : min;
        max = temp > max ? temp : max;
    }
    return std::make_pair(min, max);
}

unsigned int roundToPower2(unsigned int v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}