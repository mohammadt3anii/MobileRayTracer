//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_SCENE_HPP
#define MOBILERT_SCENE_HPP

#include "A/AABB.hpp"
#include "Intersection.hpp"
#include "Light.hpp"
#include "Primitive.hpp"
#include "Ray.hpp"
#include "Utils.hpp"
#include <vector>

namespace MobileRT {
    class Scene final {
    public:
        std::vector<Light *> lights_;
        std::vector<Primitive *> primitives_;
        AxisAlignedBoundingBox *bb;

    private:
        int traceLights(Intersection &intersection, const Ray &ray) const noexcept;

    public:
        explicit Scene() noexcept;

		Scene(const Scene &scene) noexcept = delete;

		Scene(Scene &&scene) noexcept = delete;

        ~Scene() noexcept;

		Scene &operator=(const Scene &scene) noexcept = delete;

		Scene &operator=(Scene &&scene) noexcept = delete;

        int trace(Intersection &intersection, const Ray &ray) const noexcept;

        bool shadowTrace(Intersection &intersection, Ray &&ray) const noexcept;

        static unsigned int getInstances() noexcept;

        void resetSampling() noexcept;

        void buildAccelerator() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_SCENE_HPP
