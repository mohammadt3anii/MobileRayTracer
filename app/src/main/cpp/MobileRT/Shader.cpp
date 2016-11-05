//
// Created by puscas on 16-10-2016.
//

#include "Shader.h"

using namespace MobileRT;

Shader::Shader(RayTrace &rayTrace, Scene &scene) :
        rayTrace_(rayTrace), scene_(scene) {
}

RGB Shader::Shade(const Ray &, const Intersection &) {
    return RGB();
}