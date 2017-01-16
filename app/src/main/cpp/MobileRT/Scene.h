//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SCENE_H
#define MOBILERAYTRACER_SCENE_H

#include "Lights/PointLight.h"
#include "Utils.h"
#include "Primitive.h"
#include <vector>

namespace MobileRT {
    class Scene {
    private:


    public:
        std::vector<PointLight *> lights;
        std::vector<Primitive *> primitives;

        Scene();

        ~Scene();

        bool trace(Intersection &intersection, Ray &ray) const;

        bool shadowTrace(Intersection &intersection, const Ray &ray) const;
    };
}

#endif //MOBILERAYTRACER_SCENE_H