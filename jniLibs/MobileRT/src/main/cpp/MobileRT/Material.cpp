//
// Created by Tiago on 16-10-2016.
//

#include "Material.hpp"

using MobileRT::Material;
static unsigned int counter(0u);

Material::Material() noexcept : refractiveIndice_(1.0f) {
    counter++;
}

Material::Material(const RGB &Kd, const float refractiveIndice) noexcept :
        Kd_(Kd),
        refractiveIndice_(refractiveIndice) {
    counter++;
}

Material::Material(const RGB &Kd, const RGB &Ks, const float refractiveIndice) noexcept :
        Kd_(Kd),
        Ks_(Ks),
        refractiveIndice_(refractiveIndice) {
    counter++;
}

Material::Material(const RGB &Kd, const RGB &Ks, const RGB &Kt,
                   const float refractiveIndice) noexcept :
        Kd_(Kd),
        Ks_(Ks),
        Kt_(Kt),
        refractiveIndice_(refractiveIndice) {
    counter++;
}

Material::Material(const RGB &Kd, const RGB &Ks, const RGB &Kt, const RGB &Le,
                   const float refractiveIndice) noexcept :
        Kd_(Kd),
        Ks_(Ks),
        Kt_(Kt),
        Le_(Le),
        refractiveIndice_(refractiveIndice) {
    counter++;
}

unsigned int Material::getInstances() noexcept {
    const unsigned int res(counter);
    counter = 0u;
    return res;
}
