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
            ShaderWhitted(RayTrace& rayTrace, const Scene& scene);

            void Shade(const Ray& ray, const Intersection& intersection, RGB& rgb) const override;
    };
}

#endif //MOBILERAYTRACER_SHADERWHITTED_H