//
// Created by Tiago on 14-11-2016.
//

#include "Utils.h"

//http://nghiaho.com/?p=997
float fastArcTan(const float value) {
    const float abs(value * (1.0f + (value < 0.0f) * -2.0f));
    return PI_4 * value - (value * (abs - 1.0f)) * (0.2447f + (0.0663f * abs));
}

unsigned int roundUpPower2(unsigned int value) {
    value--;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return value + 1;
}

unsigned long long int roundUpPower2(unsigned long long int value) {
    value--;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return value + 1;
}

unsigned int roundDownPower2(unsigned int value) {
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return value - (value >> 1);
}

unsigned int roundDownEvenNumber(const unsigned int value) {
    return value & ~1u;
}

unsigned int roundDownMultipleOf(unsigned int numToRound, unsigned int multipleOf) {
    if (numToRound >= multipleOf) {
        return multipleOf;
    }


    if (multipleOf == 0) {
        return numToRound;
    }

    unsigned int res(numToRound);
    for (; multipleOf % res != 0; res--);
    return res;
}