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
    class Intersection final {
    public:
        Point3D point_;   // intersection point
        Vector3D normal_;    // intersection normal
        Vector3D symNormal_;
        const Material *material_;  // material of the intersected primitive
        float length_;     // ray length parameter

    public:
        explicit Intersection(void);

        void reset(const Point3D &orig, const Vector3D &dir, const float dist,
                   const Vector3D &normal,
                   const Material &material);

        void reset(const Point3D &orig, const Vector3D &dir, const float dist,
                   const Point3D &center,
                   const Material &material);

        static unsigned int getInstances();
    };
}

#endif //MOBILERAYTRACER_INTERSECTION_H
