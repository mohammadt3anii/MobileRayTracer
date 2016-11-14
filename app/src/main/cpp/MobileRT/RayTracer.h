//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RAYTRACER_H
#define MOBILERAYTRACER_RAYTRACER_H

#include "Scenes/Scene.h"
#include "Shaders/Shader.h"

namespace MobileRT
{
    class Shader;

    class RayTracer
    {
        private:
            const Scene& scene_;
            Shader* shader_;

        public:
            RayTracer(const Scene& scene, const unsigned int shader);
            ~RayTracer();

            void rayTrace(Ray& ray, RGB& rgb, Intersection& intersection) const;
    };
}

#endif //MOBILERAYTRACER_RAYTRACER_H