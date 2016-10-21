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