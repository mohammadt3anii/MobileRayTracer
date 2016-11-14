//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADER_H
#define MOBILERAYTRACER_SHADER_H

#include "../Scenes/Scene.h"
#include "../RayTracer.h"

namespace MobileRT
{
    class RayTracer;
    
    class Shader
    {
        protected:
            const Scene& scene_;
            RayTracer& rayTracer_;

        public:
            Shader(RayTracer& rayTracer, const Scene& scene);
            virtual ~Shader();

            virtual void shade(const Ray&, Intersection&, RGB&) const;
    };
}

#endif //MOBILERAYTRACER_SHADER_H