//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_POINT3D_H
#define MOBILERAYTRACER_POINT3D_H

#include "Vector3D.h"

namespace MobileRT {
    class Vector3D;

    class Point3D final {
    public:
        float x_;
        float y_;
        float z_;

    private:
        explicit Point3D(const Point3D &point, const Vector3D &vector,
                         const float value) noexcept;

    public:
        explicit Point3D(void) noexcept;

        explicit Point3D(const float x, const float y, const float z) noexcept;

        Point3D(const Point3D &point) noexcept;

        void operator=(const Point3D &point3D) noexcept;

        const Vector3D operator-(const Point3D &point) const noexcept;

        const Point3D operator+(const Vector3D &vector) const noexcept;

        static unsigned int getInstances() noexcept;
    };
}

#endif //MOBILERAYTRACER_POINT3D_H
