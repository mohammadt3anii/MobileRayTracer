//
// Created by Tiago on 23-Jan-17.
//

#include "MobileRT/Light.hpp"

using ::MobileRT::Light;

Light::Light(Material radiance) noexcept :
        radiance_{radiance} {
}

Light::~Light() noexcept {
    LOG("LIGHT DELETED");
}
