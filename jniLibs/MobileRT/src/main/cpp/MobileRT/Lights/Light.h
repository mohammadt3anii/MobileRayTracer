//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERAYTRACER_LIGHT_H
#define MOBILERAYTRACER_LIGHT_H

#include "../RGB.h"
#include "../Point3D.h"
//#include "../Primitive.h"

namespace MobileRT {
    class Light /*: public Primitive*/ {
    public:
        RGB radiance_;

    public:
        explicit Light(const RGB &radiance);
        //explicit Light(const RGB &radiance, Shape *shape);

        virtual const Point3D getPosition(void) = 0;
        virtual ~Light (void);

        virtual void resetSampling(void) = 0;
    };
}

#endif //MOBILERAYTRACER_LIGHT_H
