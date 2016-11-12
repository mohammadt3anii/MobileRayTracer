//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SCENE_HPP
#define MOBILERAYTRACER_SCENE_HPP

#include "Light.hpp"
#include "Primitive.hpp"
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

            bool trace(const Ray& ray, Intersection& intersection) const;

            bool shadowTrace(const Ray& ray, Intersection& intersection) const;
    };
}

#endif //MOBILERAYTRACER_SCENE_HPP