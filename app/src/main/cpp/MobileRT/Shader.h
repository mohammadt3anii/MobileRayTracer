//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADER_H
#define MOBILERAYTRACER_SHADER_H

#include "Scene.h"
#include "RayTrace.h"

namespace MobileRT {
    class RayTrace;

    class Shader {
        private:

        protected:
            Scene mScene;
            RayTrace* mRT;

        public:
            Shader (RayTrace* pRT, Scene pScene);
            RGB* Shade (Ray mR, Intersection isect);
    };
}

#endif //MOBILERAYTRACER_SHADER_H

/*
package com.example.lps.mobilert;

public class Shader {
protected Scene mScene;
protected RayTrace mRT;

    Shader (RayTrace pRT, Scene pScene) {
        mScene = pScene;
        mRT = pRT;
    }

    RGB Shade (Ray mR, Intersection isect) {
        return new RGB();
    }
}
*/