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

//http://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c
int fastrand() {
    int g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}

//http://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c
static unsigned long x = 123456789, y = 362436069, z = 521288629;

unsigned long xorshf96() {//period 2^96-1
    unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;

    return z;
}

//http://en.cppreference.com/w/cpp/numeric/random