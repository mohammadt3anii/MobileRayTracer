//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RAYTRACE_H
#define MOBILERAYTRACER_RAYTRACE_H

#include "Scene.h"
#include "Shader.h"

namespace MobileRT
{
    class Shader;

    class RayTrace
    {
        private:
            const Scene& scene_;
            Shader* shader_;

        public:
            RayTrace(const Scene& pScene, const unsigned int whichShader);
            ~RayTrace();

            RGB RayV(const Ray& ray);
    };
}

#endif //MOBILERAYTRACER_RAYTRACE_H