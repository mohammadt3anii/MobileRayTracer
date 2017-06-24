//
// Created by Tiago on 16-10-2016.
//

#include "Material.h"

using namespace MobileRT;
static unsigned int counter(0u);

Material::Material(void) : refractiveIndice_(1.0f) {
    counter++;
}

// diffuse only material
Material::Material(const RGB &Kd) :
        Kd_(Kd),
        refractiveIndice_(1.0f) {
    counter++;
}

Material::Material(const RGB &Kd, const RGB &Ks) :
        Kd_(Kd),
        Ks_(Ks),
        refractiveIndice_(1.0f) {
    counter++;
}

Material::Material(const RGB &Kd, const RGB &Ks, const RGB &Kt) :
        Kd_(Kd),
        Ks_(Ks),
        Kt_(Kt),
        refractiveIndice_(1.0f) {
    counter++;
}

Material::Material(const RGB &Kd, const RGB &Ks, const RGB &Kt, const RGB &Le) :
        Kd_(Kd),
        Ks_(Ks),
        Kt_(Kt),
        Le_(Le),
        refractiveIndice_(1.0f) {
    counter++;
}

Material::Material(const RGB &Kd, const float refractiveIndice) :
        Kd_(Kd),
        refractiveIndice_(refractiveIndice) {
    counter++;
}

Material::Material(const RGB &Kd, const RGB &Ks, const float refractiveIndice) :
        Kd_(Kd),
        Ks_(Ks),
        refractiveIndice_(refractiveIndice) {
    counter++;
}

Material::Material(const RGB &Kd, const RGB &Ks, const RGB &Kt, const float refractiveIndice) :
        Kd_(Kd),
        Ks_(Ks),
        Kt_(Kt),
        refractiveIndice_(refractiveIndice) {
    counter++;
}

Material::Material(const RGB &Kd, const RGB &Ks, const RGB &Kt, const RGB &Le,
                   const float refractiveIndice) :
        Kd_(Kd),
        Ks_(Ks),
        Kt_(Kt),
        Le_(Le),
        refractiveIndice_(refractiveIndice) {
    counter++;
}

unsigned int Material::getInstances() {
    const unsigned int res(counter);
    counter = 0u;
    return res;
}
