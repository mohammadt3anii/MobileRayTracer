//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADER_H
#define MOBILERAYTRACER_SHADER_H

#include "Scene.h"
#include "RayTrace.h"

namespace MobileRT
{
    class RayTrace;
    class Shader
    {
        protected:
            Scene &scene_;
            RayTrace &rayTrace_;

        public:
            Shader(RayTrace &rayTrace, Scene &scene);

            virtual RGB Shade(const Ray &, const Intersection &) const;
    };
}

#endif //MOBILERAYTRACER_SHADER_H