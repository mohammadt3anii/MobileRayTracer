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

            virtual RGB Shade(const Ray&, const Intersection& intersection) const;
    };
}

#endif //MOBILERAYTRACER_SHADERNOSHADOWS_HPP