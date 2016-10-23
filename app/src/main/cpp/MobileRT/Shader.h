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
        virtual RGB* Shade (const Ray& r, const Intersection& isect);
    };
}

#endif //MOBILERAYTRACER_SHADER_H