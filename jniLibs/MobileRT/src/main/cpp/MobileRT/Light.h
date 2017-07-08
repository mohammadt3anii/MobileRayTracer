//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERAYTRACER_LIGHT_H
#define MOBILERAYTRACER_LIGHT_H

#include "RGB.h"
#include "Point3D.h"
#include "Intersection.h"
#include "Ray.h"

namespace MobileRT {
    class Light {
    public:
        const Material radiance_;

    public:
        explicit Light(const Material &radiance) noexcept;

        virtual ~Light(void) noexcept;

        virtual const Point3D getPosition(void) noexcept = 0;

        virtual void resetSampling(void) noexcept = 0;

        virtual bool intersect(Intersection &intersection, const Ray &ray,
                               const Material &material) const noexcept = 0;
    };
}

#endif //MOBILERAYTRACER_LIGHT_H
