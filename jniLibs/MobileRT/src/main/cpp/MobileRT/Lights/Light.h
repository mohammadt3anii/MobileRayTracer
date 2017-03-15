//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERAYTRACER_LIGHT_H
#define MOBILERAYTRACER_LIGHT_H

#include "../RGB.h"
#include "../Point3D.h"

namespace MobileRT {
    class Light {
    public:
        RGB radiance_;

    protected:
        Point3D position_;
        char padding[4] __attribute__((unused));

    public:
        explicit Light(const RGB &radiance, const Point3D &position);
        virtual Point3D getPosition (void) = 0;
        virtual ~Light (void);
    };
}

#endif //MOBILERAYTRACER_LIGHT_H
