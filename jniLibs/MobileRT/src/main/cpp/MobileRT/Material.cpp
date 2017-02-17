//
// Created by Tiago on 16-10-2016.
//

#include "Material.h"

using namespace MobileRT;

Material::Material() {
}

// diffuse only material
Material::Material(const RGB &Kd) :
        Kd_(Kd) {
}

Material::Material(const RGB &Kd, const RGB &Ks) :
        Kd_(Kd),
        Ks_(Ks) {
}
