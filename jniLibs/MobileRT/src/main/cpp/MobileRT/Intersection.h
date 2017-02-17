//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_INTERSECTION_H
#define MOBILERAYTRACER_INTERSECTION_H

#include "Point3D.h"
#include "Vector3D.h"
#include "Material.h"
#include "Utils.h"

namespace MobileRT {
    class Intersection {
    public:
        Point3D point_;   // intersection point
        Vector3D normal_;    // intersection normal
        Vector3D symNormal_;
        const Material *material_;  // material of the intersected primitive
        float length_;     // ray length parameter
        char padding[4] __attribute__((unused));

    public:
        Intersection();
        void reset(const Point3D &point, const Vector3D &normal, const float length,
                   const Material &material);
        Vector3D &getSymNormal();
    };
}

#endif //MOBILERAYTRACER_INTERSECTION_H
