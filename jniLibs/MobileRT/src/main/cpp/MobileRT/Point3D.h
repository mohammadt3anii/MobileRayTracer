//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_POINT3D_H
#define MOBILERAYTRACER_POINT3D_H

#include "Vector3D.h"

namespace MobileRT {
    class Vector3D;

    class Point3D {
    public:
        float x_;
        float y_;
        float z_;
        char padding[4] __attribute__((unused));

    public:
        explicit Point3D(void);

        explicit Point3D(const float x, const float y, const float z);

        explicit Point3D(const Point3D &point);

        Point3D(Point3D &point);

        explicit Point3D(const Point3D &point, const Vector3D &vector, const float value);

        void operator=(const Point3D &point3D);

        const Vector3D operator-(const Point3D &point) const;

        void reset(const Point3D &point, const Vector3D &vector, const float value);

        static unsigned int getInstances();
    };
}

#endif //MOBILERAYTRACER_POINT3D_H
