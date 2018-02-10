//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_SHADER_HPP
#define MOBILERT_SHADER_HPP

#include "Accelerators/BVH.hpp"
#include "Accelerators/BVH2.hpp"
#include "Accelerators/RegularGrid.hpp"
#include "Camera.hpp"
#include "Intersection.hpp"
#include "RGB.hpp"
#include "Ray.hpp"
#include "Scene.hpp"

namespace MobileRT {
  class Shader {
    public:
      Scene scene_ {};
      RegularGrid regularGrid_ {};

      BVH<MobileRT::Plane> bvhPlanes_ {};
      BVH<MobileRT::Rectangle> bvhRectangles_ {};
      BVH<MobileRT::Sphere> bvhSpheres_ {};
      BVH<MobileRT::Triangle> bvhTriangles_ {};

      BVH2<MobileRT::Plane> bvhPlanes2_ {};
      BVH2<MobileRT::Rectangle> bvhRectangles2_ {};
      BVH2<MobileRT::Sphere> bvhSpheres2_ {};
      BVH2<MobileRT::Triangle> bvhTriangles2_ {};

      enum Accelerator {
        NONE = 0,
        REGULAR_GRID,
        BOUNDING_VOLUME_HIERARCHY,
        BVH2
      };

    private:
      const Accelerator accelerator_ {};

    protected:
			const unsigned samplesLight_ {};

    protected:
			virtual bool shade (RGB *rgb, Intersection intersection,
                          Ray &&ray) noexcept = 0;

    public:
      void initializeAccelerators (Camera *camera) noexcept;

		public:
      explicit Shader (Scene &&scene, unsigned samplesLight, Accelerator accelerator) noexcept;

			Shader(const Shader &shader) noexcept = delete;

			Shader(Shader &&shader) noexcept = delete;

			virtual ~Shader() noexcept;

			Shader &operator=(const Shader &shader) noexcept = delete;

			Shader &operator=(Shader &&shader) noexcept = delete;

      bool rayTrace (RGB *rgb, Intersection *intersection, Ray &&ray) noexcept;

      bool shadowTrace (Intersection *intersection, Ray &&ray) noexcept;

      bool traceTouch (Intersection *intersection, Ray &&ray) noexcept;

			virtual void resetSampling() noexcept = 0;
  };
}//namespace MobileRT

#endif //MOBILERT_SHADER_HPP
