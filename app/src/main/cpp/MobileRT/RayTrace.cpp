//
// Created by puscas on 16-10-2016.
//

#include "RayTrace.h"
#include "ShaderNoShadows.h"
#include "ShaderWhitted.h"

using namespace MobileRT;

RayTrace::RayTrace (Scene pScene, int whichShader) {
    mScene = pScene;
    switch (whichShader) {
        case 0:
             mShader = dynamic_cast<Shader*>(new ShaderNoShadows (this, mScene));
            break;

        case 1:
            mShader = dynamic_cast<Shader*>(new ShaderWhitted (this, mScene));
            break;

        default:
            break;
    }
}

RGB* RayTrace::RayV (const Ray& r) {
    // compute radiance
    Intersection* isect;
    RGB* rad;

    isect = mScene.trace(r);
    if (isect->intersected()) {
        rad = mShader->Shade(r, *isect);
    }
    else {  // ray lost on background
        rad = new RGB(0.1f, 0.1f, 0.9f);
    }

    return rad;
}