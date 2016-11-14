//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADERNOSHADOWS_H
#define MOBILERAYTRACER_SHADERNOSHADOWS_H

#include "Shader.h"

namespace MobileRT
{
    class ShaderNoShadows : public Shader
    {
        private:
            

        public:
            ShaderNoShadows(RayTracer& rayTracer, const Scene& scene);

            void shade(const Ray&, Intersection& intersection, RGB& rgb) const;
    };
}

#endif //MOBILERAYTRACER_SHADERNOSHADOWS_H