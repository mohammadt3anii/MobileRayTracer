//
// Created by puscas on 07-04-2017.
//

#include "Constant.h"

using namespace Components;

Constant::Constant(const float value) :
        Sampler(0ull, 0u),
        value_(value) {
}

float Constant::getSample(const unsigned int) {
    return value_;
}
