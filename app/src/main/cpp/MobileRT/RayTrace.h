//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RAYTRACE_H
#define MOBILERAYTRACER_RAYTRACE_H

#include <bits/unique_ptr.h>
#include "Scene.h"
#include "Shader.h"

namespace MobileRT
{
    class Shader;

    class RayTrace
    {
        private:
            Scene &scene_;
            std::unique_ptr<Shader> shader_;

        public:
            RayTrace(Scene &pScene, const int &whichShader);

            RGB RayV(const Ray &ray);
    };
}

#endif //MOBILERAYTRACER_RAYTRACE_H