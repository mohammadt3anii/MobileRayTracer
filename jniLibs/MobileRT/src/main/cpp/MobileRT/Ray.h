//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RAY_H
#define MOBILERAYTRACER_RAY_H

#include "Point3D.h"
#include "Vector3D.h"
#include "Utils.h"

namespace MobileRT {
    class Ray final {
    public:
        const Point3D origin_;
        const Vector3D direction_;
        const Vector3D symDirection_;
        const unsigned int depth_;

    public:
        explicit Ray(void);

        explicit Ray(const float x, const float y, const float z, const Point3D &position,
                     const unsigned int depth);

        explicit Ray(const Vector3D &dir, const Point3D &origin,
                     const unsigned int depth);

        static unsigned int getInstances();
    };
}

#endif //MOBILERAYTRACER_RAY_H
