//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERAYTRACER_CAMERA_H
#define MOBILERAYTRACER_CAMERA_H

#include "Point3D.h"
#include "Ray.h"

namespace MobileRT {
    class Camera {
    protected:
        Point3D position_;
        Point3D lookAt_;
        Vector3D direction_;
        Vector3D right_;
        Vector3D up_;

    public:
        explicit Camera(const Point3D &position, const Point3D &lookAt, const Vector3D &up);

        virtual Ray generateRay(const float u, const float v,
                                const float deviationU, const float deviationV) const = 0;

        virtual ~Camera(void);

        virtual void reset(const Point3D &position, const Point3D &lookAt, const Vector3D &up,
                           const float hFov, const float vFov) = 0;
    };
}

#endif //MOBILERAYTRACER_CAMERA_H
