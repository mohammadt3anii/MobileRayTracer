//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_PATHTRACER_H
#define MOBILERAYTRACER_PATHTRACER_H

#include "Shader.h"

namespace MobileRT {
    class PathTracer : public Shader {
    private:
        enum {
            MAX_DEPTH = 5
        };

    public:
        PathTracer(RayTracer &rayTracer, const Scene &scene);

        void shade(RGB &rgb, Intersection &intersection, const Ray &ray,
                   Vector3D &vectIntersectCamera) const override;
    };
}

#endif //MOBILERAYTRACER_PATHTRACER_H