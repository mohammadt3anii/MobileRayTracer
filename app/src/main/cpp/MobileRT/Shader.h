//
// Created by Tiago on 16-10-2016.
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

            virtual void Shade(const Ray&, const Intersection&, RGB&) const;
    };
}

#endif //MOBILERAYTRACER_SHADER_H