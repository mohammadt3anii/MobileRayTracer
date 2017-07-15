//
// Created by Tiago on 14-11-2016.
//

#include "Utils.hpp"

//http://nghiaho.com/?p=997
float fastArcTan(const float value) noexcept {
    const float absValue(value < 0.0f? -value : value);
	//print("1", 2, "3", 4.0f);
    return PI_4 * value - (value * (absValue - 1.0f)) * (0.2447f + (0.0663f * absValue));
}

unsigned int roundDownToMultipleOf(const unsigned int value, const unsigned int multiple) noexcept {
    const unsigned int rest(value % multiple);
    return rest > 0? value - rest : value;
}
