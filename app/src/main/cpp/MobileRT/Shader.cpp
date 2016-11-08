//
// Created by puscas on 16-10-2016.
//

#include "Shader.h"

using namespace MobileRT;

Shader::Shader(RayTrace &rayTrace, Scene &scene) :
        scene_(scene),
        rayTrace_(rayTrace)
{
}

RGB Shader::Shade(const Ray &, const Intersection &) const
{
    return RGB();
}