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
#include "Shapes/Plane.hpp"
#include "Shapes/Rectangle.hpp"
#include "Shapes/Sphere.hpp"
#include "Shapes/Triangle.hpp"
#include "Utils.hpp"
#include <vector>

namespace MobileRT {
    class Scene final {
    public:
      std::vector<Primitive<Triangle>> triangles_ {std::vector<Primitive<Triangle>>{}};
      std::vector<Primitive<Sphere>> spheres_ {std::vector<Primitive<Sphere>>{}};
      std::vector<Primitive<Plane>> planes_ {std::vector<Primitive<Plane>>{}};
      std::vector<Primitive<Rectangle>> rectangles_ {std::vector<Primitive<Rectangle>>{}};
			std::vector<Light *> lights_ {std::vector<Light *>{}};

    private:
      template<typename T>
      int trace (const std::vector<T> &primitives, Intersection *intersection, Ray ray,
                 int res) const noexcept;

      template<typename T>
      bool shadowTrace(const std::vector<T> &primitives, Intersection *intersection,
                       Ray ray) const noexcept;

    public:
			explicit Scene () = default;
      Scene (const Scene &scene) noexcept = delete;
      Scene (Scene &&scene) noexcept = default;
      ~Scene () noexcept;
      Scene &operator= (const Scene &scene) noexcept = delete;
      Scene &operator= (Scene &&scene) noexcept = default;

      int trace(Intersection *intersection, Ray ray) const noexcept;
      bool shadowTrace (Intersection *intersection, Ray &&ray) const noexcept;
      static unsigned getInstances () noexcept;
      int traceLights (Intersection *intersection, Ray ray) const noexcept;
      void resetSampling () noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_SCENE_HPP
