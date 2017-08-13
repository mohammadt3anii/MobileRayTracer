//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_INTERSECTION_HPP
#define MOBILERT_INTERSECTION_HPP

#include "Material.hpp"
#include "Point3D.hpp"
#include "Utils.hpp"
#include "Vector3D.hpp"

namespace MobileRT {
    class Intersection final {
    public:
      Point3D point_ {};   // intersection point
      Vector3D normal_ {};    // intersection normal
      Vector3D symNormal_ {};
			const Material *material_ {nullptr};// material of the intersected primitive
			float length_ {RayLengthMax};// ray length parameter

    public:
			explicit Intersection () noexcept = default;
			Intersection (const Intersection &intersection) noexcept = default;

		Intersection(Intersection &&intersection) noexcept = default;

		~Intersection() noexcept = default;

		Intersection &operator=(const Intersection &intersection) noexcept = delete;

		Intersection &operator=(Intersection &&intersection) noexcept = delete;
			void reset (Point3D orig, Vector3D dir, float dist,
									Vector3D normal) noexcept;
			void reset (Point3D orig, Vector3D dir, float dist,
									Point3D center) noexcept;
      static unsigned getInstances () noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_INTERSECTION_HPP
