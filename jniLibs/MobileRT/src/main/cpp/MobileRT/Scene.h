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
        //Light* lights_;
        std::vector<Primitive *> primitives_;
        //Primitive* primitives_;
        //unsigned int sizeLights_;
        unsigned int sizePrimitives_;
        char padding[4] __attribute__((unused));
        std::function<bool(Intersection &, const Ray &)> *intersect_;

    public:
        explicit Scene(void);

        /*explicit Scene(const unsigned int sizeLights, Light* lights,
                       const unsigned int sizePrimitives, Primitive* primitives);*/
        ~Scene(void);

        bool trace(Intersection &intersection, Ray &ray) const;
        bool shadowTrace(Intersection &intersection, const Ray &ray) const;

        void cache();

        static unsigned int getInstances();
    };
}

#endif //MOBILERAYTRACER_SCENE_H
