//
// Created by Tiago on 14-11-2016.
//

#include "Utils.h"

//http://nghiaho.com/?p=997
float fastArcTan(const float value) {
    const float abs((value < 0) ? -value : value);
    return PI_4 * value - (value * (abs - 1.0f)) * (0.2447f + (0.0663f * abs));
}

//https://en.wikipedia.org/wiki/Halton_sequence
float haltonSequence(const unsigned int index, const unsigned int base) {
    float f(1.0f);
    float result(0.0f);
    float i(index);
    while (i > 0.0f) {
        f = f / base;
        result = result + f * (static_cast<unsigned int> (i) % base);
        i = std::floor(i / base);
    }
    return result;
}

unsigned int roundToPower2(unsigned int number) {
    number--;
    number |= number >> 1;
    number |= number >> 2;
    number |= number >> 4;
    number |= number >> 8;
    number |= number >> 16;
    number++;
    return number;
}

unsigned int roundToEvenNumber(unsigned int number) {
    return number & static_cast<unsigned int> (~1);
    //return number - (number & 1);;
    //return (number % 2 == 0) ? number : (number + 1);
}
