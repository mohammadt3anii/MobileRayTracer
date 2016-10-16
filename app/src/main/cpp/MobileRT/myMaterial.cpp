//
// Created by puscas on 16-10-2016.
//

#include "myMaterial.h"

using namespace MobileRT;

myMaterial::myMaterial () {
    Kd = new RGB();
    Ks = new RGB();
}

myMaterial::myMaterial (RGB* pKd) { // diffuse only material
    Kd = pKd;
    Ks = new RGB();
}

myMaterial::myMaterial (RGB *pKd, RGB *pKs) { // diffuse only material
    Kd = pKd;
    Ks = pKs;
}