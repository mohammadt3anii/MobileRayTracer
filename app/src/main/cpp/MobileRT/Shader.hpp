//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADER_HPP
#define MOBILERAYTRACER_SHADER_HPP

#include "Scene.hpp"
#include "RayTrace.hpp"

namespace MobileRT
{
    class RayTrace;
    
    class Shader
    {
        protected:
            const Scene& scene_;
            RayTrace& rayTrace_;

        public:
            Shader(RayTrace& rayTrace, const Scene& scene);
            virtual ~Shader();

            virtual void Shade(const Ray&, const Intersection&, RGB&) const;
    };
}

#endif //MOBILERAYTRACER_SHADER_HPP