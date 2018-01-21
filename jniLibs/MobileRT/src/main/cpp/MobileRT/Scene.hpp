//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_SCENE_HPP
#define MOBILERT_SCENE_HPP

#include "Accelerators/AABB.hpp"
#include "Intersection.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Primitive.hpp"
#include "Ray.hpp"
#include "Utils.hpp"
#include <vector>

namespace MobileRT {
    class Scene final {
    public:
      ::std::vector<Primitive<Triangle>> triangles_ {::std::vector<Primitive<Triangle>>{}};
      ::std::vector<Primitive<Sphere>> spheres_ {::std::vector<Primitive<Sphere>>{}};
      ::std::vector<Primitive<Plane>> planes_ {::std::vector<Primitive<Plane>>{}};
      ::std::vector<Primitive<Rectangle>> rectangles_ {::std::vector<Primitive<Rectangle>>{}};
			::std::vector<::std::unique_ptr<Light>> lights_ {::std::vector<::std::unique_ptr<Light>>{}};

    private:
      template<typename T>
      bool trace (::std::vector<T> &primitives, Intersection *intersection, Ray ray) noexcept;

      template<typename T>
      bool shadowTrace (::std::vector<T> &primitives, Intersection *intersection,
                        Ray ray) const noexcept;
      static void AABBbounds (AABB box, Point3D *min, Point3D *max);

    public:
			explicit Scene () = default;

      Scene (const Scene &scene) noexcept = delete;

      Scene (Scene &&scene) noexcept = default;
      ~Scene () noexcept;
      Scene &operator= (const Scene &scene) noexcept = delete;
      Scene &operator= (Scene &&scene) noexcept = default;
      bool trace (Intersection *intersection, Ray ray) noexcept;
      bool shadowTrace (Intersection *intersection, Ray &&ray) noexcept;

      static unsigned getInstances () noexcept;
      bool traceLights (Intersection *intersection, Ray ray) const noexcept;
      void resetSampling () noexcept;

      template<typename T>
      static void getBounds (::std::vector<T *> primitives, Point3D *const min, Point3D *const max) {
        for (T *primitive : primitives) {
          AABBbounds (primitive->getAABB (), min, max);
        }
      }
    };
}//namespace MobileRT

#endif //MOBILERT_SCENE_HPP
