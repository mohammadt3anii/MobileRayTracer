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
            shader_ = std::unique_ptr<ShaderNoShadows>(new ShaderNoShadows(*this, scene));
        }
            break;
        case 1: {
            shader_ = std::unique_ptr<ShaderWhitted>(new ShaderWhitted(*this, scene));
        }
            break;
    }
}

RGB RayTrace::RayV(const Ray &ray) {
    // compute radiance
    Intersection intersection(std::move(scene_.trace(ray)));
    if (intersection.intersected()) {
        return shader_->Shade(ray, intersection);
    }
    return RGB(0.1f, 0.1f, 0.9f);
}