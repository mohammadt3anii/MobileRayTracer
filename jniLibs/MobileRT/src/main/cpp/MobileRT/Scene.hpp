//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SCENE_H
#define MOBILERAYTRACER_SCENE_H

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

        ~Scene() noexcept;

        int trace(Intersection &intersection, const Ray &ray) const noexcept;

        bool shadowTrace(Intersection &intersection, const Ray &ray) const noexcept;

        static unsigned int getInstances() noexcept;

        void resetSampling() noexcept;

        void buildAccelerator() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_SCENE_H
