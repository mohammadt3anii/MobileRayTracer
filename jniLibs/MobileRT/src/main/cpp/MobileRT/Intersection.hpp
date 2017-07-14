//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_INTERSECTION_H
#define MOBILERAYTRACER_INTERSECTION_H

#include "Material.hpp"
#include "Point3D.hpp"
#include "Utils.hpp"
#include "Vector3D.hpp"

namespace MobileRT {
    class Intersection final {
    public:
        Point3D point_;   // intersection point
        Vector3D normal_;    // intersection normal
        Vector3D symNormal_;
        const Material *material_;  // material of the intersected primitive
        float length_;     // ray length parameter

    public:
        explicit Intersection() noexcept;

        void reset(const Point3D &orig, const Vector3D &dir, float dist,
                   const Vector3D &normal,
                   const Material &material) noexcept;

        void reset(const Point3D &orig, const Vector3D &dir, float dist,
                   const Point3D &center,
                   const Material &material) noexcept;

        static unsigned int getInstances() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_INTERSECTION_H
