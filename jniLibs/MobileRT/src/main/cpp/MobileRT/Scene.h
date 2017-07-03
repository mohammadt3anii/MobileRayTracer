//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SCENE_H
#define MOBILERAYTRACER_SCENE_H

#include "Light.h"
#include "Primitive.h"
#include "Intersection.h"
#include "Ray.h"
#include "Utils.h"
#include <vector>

namespace MobileRT {
    class Scene final {
    public:
        std::vector<Light *> lights_;
        std::vector<Primitive *> primitives_;

    private:
        int traceLights(Intersection &intersection, const Ray &ray) const noexcept;

    public:
        explicit Scene(void) noexcept;

        ~Scene(void) noexcept;

        int trace(Intersection &intersection, const Ray &ray) const noexcept;

        bool shadowTrace(Intersection &intersection, const Ray &ray) const noexcept;

        static unsigned int getInstances(void) noexcept;

        void resetSampling(void) noexcept;
    };
}

#endif //MOBILERAYTRACER_SCENE_H
