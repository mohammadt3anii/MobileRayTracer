//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADERWHITTED_HPP
#define MOBILERAYTRACER_SHADERWHITTED_HPP

#include "Shader.hpp"

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

#endif //MOBILERAYTRACER_SHADERWHITTED_HPP