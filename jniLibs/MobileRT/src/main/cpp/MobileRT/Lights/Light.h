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

    public:
        explicit Light(const RGB &radiance, const Point3D &position);

        virtual Point3D getPosition(const unsigned int i, const unsigned int j) = 0;
        virtual ~Light (void);

        virtual void resetSampling(void) = 0;
    };
}

#endif //MOBILERAYTRACER_LIGHT_H
