//
// Created by Tiago on 16-10-2016.
//

#include "Material.h"

using namespace MobileRT;
static unsigned int counter = 0;

Material::Material(void) {
    counter++;
}

// diffuse only material
Material::Material(const RGB &Kd) :
        Kd_(Kd) {
    counter++;
}

Material::Material(const RGB &Kd, const RGB &Ks) :
        Kd_(Kd),
        Ks_(Ks) {
    counter++;
}

unsigned int Material::getInstances() {
    unsigned int res(counter);
    counter = 0;
    return res;
}
