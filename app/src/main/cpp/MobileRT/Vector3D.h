//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_VECTOR3D_H
#define MOBILERAYTRACER_VECTOR3D_H

#include "Point3D.h"

namespace MobileRT {
    class Vector3D {
    private:


    public:
        float x_;
        float y_;
        float z_;

        Vector3D();

        Vector3D(const float x, const float y, const float z);

        Vector3D(const Vector3D &vector);

        // symetric vector
        Vector3D negative() const;

        // dot product
        float dotProduct(const Vector3D &vector) const;

        float dotProduct(const Point3D &point) const;

        float squareLength() const;

        Vector3D crossProduct(const Vector3D &vector) const;

        void mult(const float value);

        void sub(const Vector3D &vector);

        float length() const;

        float normalize();

        Vector3D returnNormalized() const;

        Vector3D operator*(const float value) const;

        void recycle(const float x, const float y, const float z);

        void recycle(const Point3D &dest, const Point3D &orig);
    };
}

#endif //MOBILERAYTRACER_VECTOR3D_H