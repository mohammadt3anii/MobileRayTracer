//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADERWHITTED_H
#define MOBILERAYTRACER_SHADERWHITTED_H

#include "Shader.h"

namespace MobileRT
{
    class ShaderWhitted : public Shader
    {
        private:
            enum{
                MAX_DEPTH = 5
            };

        public:
            ShaderWhitted(RayTracer& rayTracer, const Scene& scene);

            void shade(const Ray& ray, Intersection& intersection, RGB& rgb) const override;
    };
}

#endif //MOBILERAYTRACER_SHADERWHITTED_H