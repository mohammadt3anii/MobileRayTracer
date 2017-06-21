//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_VECTOR3D_H
#define MOBILERAYTRACER_VECTOR3D_H

#include "Point3D.h"
#include "Utils.h"

namespace MobileRT {
    class Point3D;

    class Vector3D {
    public:
        float x_;
        float y_;
        float z_;

    public:
        explicit Vector3D(void);

        explicit Vector3D(const float x, const float y, const float z);
        Vector3D(const Vector3D &vector);
        explicit Vector3D(const Vector3D &vector, const float value);

        explicit Vector3D(const Point3D &dest, const Point3D &orig);

        explicit Vector3D(const Point3D &dest, const Point3D &orig, bool);

        explicit Vector3D(const Vector3D &vector1, const Vector3D &vector2);

        const Vector3D &operator=(const Vector3D &vector);

        const Vector3D operator*(const float value);

        const Vector3D operator+(const Vector3D vector) const;
        float dotProduct(const Vector3D &vector) const;

        float dotProduct(const Point3D &dest, const Point3D &orig) const;

        float squareMagnitude(void) const;

        float magnitude(void) const;
        Vector3D crossProduct(const Vector3D &vector) const;
        void mult(const float value);

        void subAndNormalize(const Vector3D &vector);

        float normalize(void);

        const Vector3D returnNormalized(void) const;

        const Vector3D operator*(const float value) const;
        void reset(const float x, const float y, const float z);
        void reset(const Point3D &dest, const Point3D &orig);

        Vector3D symmetric(void) const;

        static unsigned int getInstances();
    };
}

#endif //MOBILERAYTRACER_VECTOR3D_H
