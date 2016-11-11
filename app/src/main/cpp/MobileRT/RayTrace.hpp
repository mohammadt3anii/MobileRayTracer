//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RAYTRACE_HPP
#define MOBILERAYTRACER_RAYTRACE_HPP

#include "Scene.hpp"
#include "Shader.hpp"

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

#endif //MOBILERAYTRACER_RAYTRACE_HPP