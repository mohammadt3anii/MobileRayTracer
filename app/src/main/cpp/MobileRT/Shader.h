//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADER_H
#define MOBILERAYTRACER_SHADER_H

#include "Scene.h"
#include "RayTrace.h"

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

            virtual RGB Shade(const Ray&, const Intersection&) const;
    };
}

#endif //MOBILERAYTRACER_SHADER_H