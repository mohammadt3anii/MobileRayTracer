//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERAYTRACER_CAMERA_H
#define MOBILERAYTRACER_CAMERA_H

#include "Point3D.h"
#include "Ray.h"

namespace MobileRT {
    class Camera {
		public:
			Point3D position_;
			Vector3D direction_;
			Vector3D right_;
			Vector3D up_;

		public:
			explicit Camera(const Point3D &position, const Point3D &lookAt, const Vector3D &up);

			virtual const Ray generateRay(const float u, const float v,
										const float deviationU, const float deviationV) const = 0;

			virtual ~Camera(void);
    };
}

#endif //MOBILERAYTRACER_CAMERA_H
