//
// Created by Tiago on 23-Jan-17.
//

#include "Light.h"

using namespace MobileRT;

Light::Light(const Material &radiance) noexcept :
        radiance_(radiance) {
}

Light::~Light(void) noexcept {
}
