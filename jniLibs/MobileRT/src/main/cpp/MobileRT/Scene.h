//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SCENE_H
#define MOBILERAYTRACER_SCENE_H

#include "Lights/Light.h"
#include "Primitive.h"
#include "Intersection.h"
#include "Ray.h"
#include "Utils.h"
#include <vector>

namespace MobileRT {
    class Scene {
    public:
        std::vector<Light *> lights_;
        std::vector<Primitive *> primitives_;

    public:
        explicit Scene(void);

        ~Scene(void);

        int traceLights(Intersection &intersection, Ray &ray) const;
        int trace(Intersection &intersection, Ray &ray) const;
        bool shadowTrace(Intersection &intersection, const Ray &ray) const;

        static unsigned int getInstances(void);

        void resetSampling(void);
    };
}

#endif //MOBILERAYTRACER_SCENE_H
