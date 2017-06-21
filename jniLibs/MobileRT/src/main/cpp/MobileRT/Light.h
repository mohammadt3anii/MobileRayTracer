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
        Material radiance_;

    public:
        explicit Light(const Material &radiance);
        //explicit Light(const RGB &radiance, Shape *shape);

        virtual const Point3D getPosition(void) = 0;

        virtual ~Light(void);

        virtual void resetSampling(void) = 0;

        virtual bool
        intersect(Intersection &intersection, const Ray &ray, const Material &material) const = 0;
    };
}

#endif //MOBILERAYTRACER_LIGHT_H
