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
        Whitted(const Scene &scene);

        void shade(RGB &rgb,
                   Intersection &intersection,
                   const Ray &ray,
                   Vector3D &vectorIntersectCamera,
                   RayTraceCall rayTraceCall) const override;
    };
}

#endif //MOBILERAYTRACER_WHITTED_H