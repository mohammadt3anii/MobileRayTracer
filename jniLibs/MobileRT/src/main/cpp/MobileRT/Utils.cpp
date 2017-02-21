//
// Created by Tiago on 14-11-2016.
//

#include "Utils.h"

//http://nghiaho.com/?p=997
float fastArcTan(const float value) {
    const float abs(value * (1 + (value < 0) * -2));
    return PI_4 * value - (value * (abs - 1.0f)) * (0.2447f + (0.0663f * abs));
}

unsigned int roundToPower2(unsigned int value) {
    value--;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value++;
    return value;
}

unsigned int roundToEvenNumber(const unsigned int value) {
    return value & ~1u;
}
