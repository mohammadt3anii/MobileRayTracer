//
// Created by Tiago on 23-Jan-17.
//

#include "Light.hpp"

using MobileRT::Light;

Light::Light (Material radiance) noexcept :
  radiance_ {std::move (radiance)} {
}

Light::~Light() noexcept {
	LOG("LIGHT DELETED");
}
