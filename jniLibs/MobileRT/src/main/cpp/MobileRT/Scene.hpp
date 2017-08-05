//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_SCENE_HPP
#define MOBILERT_SCENE_HPP

#include "Accelerators/AABB.hpp"
#include "Intersection.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include "Shapes/Plane.hpp"
#include "Shapes/Sphere.hpp"
#include "Shapes/Triangle.hpp"
#include "Utils.hpp"
#include <vector>

namespace MobileRT {
    class Scene final {
    public:
			std::vector<Triangle> triangles_;
			std::vector<Sphere> spheres_;
			std::vector<Plane> planes_;

			std::vector<Material> materials_;
			std::vector<Light *> lights_;

    private:
      int traceLights(Intersection *intersection, Ray ray) const noexcept;

      private:
      template<typename T>
      int trace(const std::vector<T> &primitives, Intersection *intersection, Ray ray, int offset,
                int res) const noexcept;

      template<typename T>
      bool shadowTrace(const std::vector<T> &primitives, Intersection *intersection,
                       Ray ray) const noexcept;

    public:
        explicit Scene() noexcept;

		Scene(const Scene &scene) noexcept = delete;

		Scene(Scene &&scene) noexcept = default;

        ~Scene() noexcept;

		Scene &operator=(const Scene &scene) noexcept = delete;

		Scene &operator=(Scene &&scene) noexcept = default;

      int trace(Intersection *intersection, Ray ray) const noexcept;

        bool shadowTrace(Intersection *intersection, Ray &&ray) const noexcept;

        static unsigned int getInstances() noexcept;

        void resetSampling() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_SCENE_HPP
