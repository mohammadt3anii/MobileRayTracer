//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_NOSHADOWS_H
#define MOBILERAYTRACER_NOSHADOWS_H

#include "Shader.h"

namespace MobileRT
{
    class NoShadows : public Shader
    {
        private:
            

        public:
            NoShadows(RayTracer& rayTracer, const Scene& scene);

        void shade(RGB &rgb, Intersection &intersection, const Ray &,
                   Vector3D &vectIntersectCamera) const override;
    };
}

#endif //MOBILERAYTRACER_NOSHADOWS_H