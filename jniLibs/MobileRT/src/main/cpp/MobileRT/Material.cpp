//
// Created by Tiago on 16-10-2016.
//

#include "Material.hpp"

using MobileRT::Material;
static unsigned int counter(0u);

Material::Material() noexcept : refractiveIndice_(1.0f) {
    counter++;
}

Material::Material(RGB Kd, const float refractiveIndice) noexcept :
        Kd_(std::move(Kd)),
        refractiveIndice_(refractiveIndice) {
    counter++;
}

Material::Material(RGB Kd, RGB Ks, const float refractiveIndice) noexcept :
        Kd_(std::move(Kd)),
        Ks_(std::move(Ks)),
        refractiveIndice_(refractiveIndice) {
    counter++;
}

Material::Material(RGB Kd, RGB Ks, RGB Kt,
                   const float refractiveIndice) noexcept :
        Kd_(std::move(Kd)),
        Ks_(std::move(Ks)),
        Kt_(std::move(Kt)),
        refractiveIndice_(refractiveIndice) {
    counter++;
}

Material::Material(RGB Kd, RGB Ks, RGB Kt, RGB Le,
                   const float refractiveIndice) noexcept :
        Kd_(std::move(Kd)),
        Ks_(std::move(Ks)),
        Kt_(std::move(Kt)),
        Le_(std::move(Le)),
        refractiveIndice_(refractiveIndice) {
    counter++;
}

unsigned int Material::getInstances() noexcept {
    const unsigned int res(counter);
    counter = 0u;
    return res;
}
