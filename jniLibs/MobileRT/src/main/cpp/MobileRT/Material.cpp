//
// Created by Tiago on 16-10-2016.
//

#include "Material.hpp"

using MobileRT::Material;

static unsigned int counter(0);

Material::Material(RGB Kd, RGB Ks, RGB Kt, const float refractiveIndice, RGB Le)noexcept :
        Le_(std::move(Le)),
        Kd_(std::move(Kd)),
        Ks_(std::move(Ks)),
        Kt_(std::move(Kt)),
        refractiveIndice_(refractiveIndice)
{
    counter++;
}

unsigned int Material::getInstances() noexcept {
    const unsigned int res(counter);
    counter = 0;
    return res;
}
