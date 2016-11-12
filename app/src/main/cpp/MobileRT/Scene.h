//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SCENE_H
#define MOBILERAYTRACER_SCENE_H

#include "Light.h"
#include "Primitive.h"
#include <vector>

namespace MobileRT
{
    class Scene
    {
        private:


    public:
            std::vector<Light*> lights;
            std::vector<Primitive*> primitives;

            Scene ();
            ~Scene ();

            bool trace(Ray& ray, Intersection& intersection) const;

            bool shadowTrace(const Ray& ray, Intersection& intersection) const;
    };
}

#endif //MOBILERAYTRACER_SCENE_H