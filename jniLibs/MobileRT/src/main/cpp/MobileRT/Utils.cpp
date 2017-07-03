//
// Created by Tiago on 14-11-2016.
//

#include "Utils.h"

//http://nghiaho.com/?p=997
float fastArcTan(const float value) noexcept {
    const float abs(value * (1.0f + (value < 0.0f) * -2.0f));
    return PI_4 * value - (value * (abs - 1.0f)) * (0.2447f + (0.0663f * abs));
}

unsigned int roundDownToMultipleOf(const unsigned int value, const unsigned int multiple) noexcept {
    const unsigned int rest(value % multiple);
    return rest ? value - rest : value;
}
