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
			Point3D origin_;

		public:
			explicit Intersection(void);

			explicit Intersection(const Intersection &intersection);
			
			void reset(const Point3D &point,
					   const Vector3D &normal,
					   const float dist,
					   const Material &material,
					   const Point3D &origin);

			void reset(const float x, const float y, const float z,
					   const Vector3D &normal,
					   const float dist,
					   const Material &material,
					   const Point3D &origin);

			void reset(const Point3D &orig,
					   const Vector3D &dir,
					   const float dist,
					   const Vector3D &normal,
					   const Material &material,
					   const Point3D &origin);

			static unsigned int getInstances();
    };
}

#endif //MOBILERAYTRACER_INTERSECTION_H
