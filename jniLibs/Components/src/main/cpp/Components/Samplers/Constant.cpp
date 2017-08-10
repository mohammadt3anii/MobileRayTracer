//
// Created by puscas on 07-04-2017.
//

#include "Constant.hpp"

using Components::Constant;

Constant::Constant(const float value) noexcept :
  value_(value) {
}

float Constant::getSample(const unsigned int /*sample*/) noexcept {
    return value_;
}
