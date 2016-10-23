//
// Created by puscas on 16-10-2016.
//

#include "Shader.h"
#include "RGB.h"

using namespace MobileRT;

Shader::Shader (RayTrace* pRT, Scene pScene) {
    mScene = pScene;
    mRT = pRT;
}

RGB* Shader::Shade (const Ray& mR, const Intersection& isect) {
    return new RGB();
}