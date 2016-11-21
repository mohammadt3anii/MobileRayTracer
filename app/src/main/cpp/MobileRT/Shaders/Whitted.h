//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_WHITTED_H
#define MOBILERAYTRACER_WHITTED_H

#include "Shader.h"

namespace MobileRT {
    class Whitted : public Shader {
    private:
        enum {
            MAX_DEPTH = 5
        };

    public:
        Whitted(RayTracer &rayTracer, const Scene &scene);

        void shade(RGB &rgb, Intersection &intersection, const Ray &ray,
                   Vector3D &vectIntersectCamera) const override;
    };
}

#endif //MOBILERAYTRACER_WHITTED_H