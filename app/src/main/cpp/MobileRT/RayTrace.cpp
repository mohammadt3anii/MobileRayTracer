//
// Created by puscas on 16-10-2016.
//

#include "RayTrace.h"
#include "ShaderNoShadows.h"
#include "ShaderWhitted.h"

using namespace MobileRT;

RayTrace::RayTrace(Scene &scene, const int &whichShader) :
        scene_(scene) {
    switch (whichShader) {
        case 0: {
            shader_ = std::unique_ptr<ShaderNoShadows>(new ShaderNoShadows(this, scene));
        }
            break;
        case 1: {
            shader_ = std::unique_ptr<ShaderWhitted>(new ShaderWhitted(this, scene));
        }
            break;
    }
}

RGB* RayTrace::RayV (const Ray& r) {
    // compute radiance
    Intersection* isect;
    RGB* rad;

    isect = scene_.trace(r);
    if (isect->intersected()) {
        rad = shader_->Shade(r, *isect);
    }
    else {  // ray lost on background
        rad = new RGB(0.1f, 0.1f, 0.9f);
    }

    return rad;
}