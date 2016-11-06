//
// Created by puscas on 16-10-2016.
//

#include "Material.h"

using namespace MobileRT;

Material::Material ()
{
}

// diffuse only material
Material::Material (const RGB& pKd) :
    Kd(pKd)
{
}

Material::Material (const RGB& pKd, const RGB& pKs) :
    Kd(pKd),
    Ks(pKs)
{
}