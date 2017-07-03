//
// Created by puscas on 07-04-2017.
//

#include "Constant.h"

using namespace Components;

Constant::Constant(const float value) noexcept :
        Sampler(0ull, 0u),
        value_(value) {
}

float Constant::getSample(const unsigned int) noexcept {
    return value_;
}
