//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADERNOSHADOWS_HPP
#define MOBILERAYTRACER_SHADERNOSHADOWS_HPP

#include "Shader.hpp"

namespace MobileRT
{
    class ShaderNoShadows : public Shader
    {
        private:
            
            
        public:
            ShaderNoShadows(RayTrace& rayTrace, const Scene& scene);

            void Shade(const Ray&, const Intersection& intersection, RGB& rgb) const;
    };
}

#endif //MOBILERAYTRACER_SHADERNOSHADOWS_HPP