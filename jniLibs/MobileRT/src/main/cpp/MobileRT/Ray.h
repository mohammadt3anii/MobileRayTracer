//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RAY_H
#define MOBILERAYTRACER_RAY_H

#include "Point3D.h"
#include "Vector3D.h"
#include "Utils.h"

namespace MobileRT {
    class Ray {
    private:


    public:
        Point3D origin_;
        Vector3D direction_;
        float maxDistance_;
        const unsigned int depth_;
        Vector3D symDirection_;

        Ray();

        Ray(const float x, const float y, const float z, Point3D position);

        Ray(const Point3D &orig, const Vector3D &dir, const float maxDist,
            const unsigned int depth);

        void reset(const float x, const float y, const float z, const Point3D &origin);
    };
}

#endif //MOBILERAYTRACER_RAY_H