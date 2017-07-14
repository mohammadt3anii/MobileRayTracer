//
// Created by Tiago on 23-Jan-17.
//

#include "Light.hpp"

using MobileRT::Light;

Light::Light(const Material &radiance) noexcept :
        radiance_(radiance) {
}
