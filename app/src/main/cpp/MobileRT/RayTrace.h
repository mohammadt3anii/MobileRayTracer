//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RAYTRACE_H
#define MOBILERAYTRACER_RAYTRACE_H

#include "Scene.h"
#include "Shader.h"

namespace MobileRT {
    class Shader;
    class RayTrace {
        private:
            Scene mScene;
            Shader* mShader;

        public:
            RayTrace (Scene pScene, int whichShader);
            RGB* RayV (Ray r);
    };
}

#endif //MOBILERAYTRACER_RAYTRACE_H

/*
package com.example.lps.mobilert;

public class RayTrace {
private Scene mScene;
private Shader mShader;

    RayTrace (Scene pScene, int whichShader) {
        mScene = pScene;
        switch (whichShader) {
            case 0:
                mShader = new ShaderNoShadows (this, mScene);
                break;
            case 1:
                mShader = new ShaderWhitted (this, mScene);
                break;

        }
    }

public RGB RayV (Ray r) {
        // compute radiance
        Intersection isect;
        final RGB background = new RGB(0.1f, 0.1f, 0.9f);
        RGB rad;

        isect = mScene.trace (r);
        if (isect.intersected) {
            rad = mShader.Shade(r, isect);
        }
        else {  // ray lost on background
            rad = background;
        }

        return rad;
    }

}
*/