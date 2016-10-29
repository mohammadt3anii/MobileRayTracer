//
// Created by puscas on 16-10-2016.
//

#include "Material.h"

using namespace MobileRT;

Material::Material () {
    Kd = new RGB();
    Ks = new RGB();
}

Material::Material (RGB* pKd) { // diffuse only material
    Kd = pKd;
    Ks = new RGB();
}

Material::Material (RGB *pKd, RGB *pKs) { // diffuse only material
    Kd = pKd;
    Ks = pKs;
}