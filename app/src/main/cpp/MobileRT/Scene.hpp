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

            const Intersection* trace(const Ray& ray) const;

            const Intersection* shadowTrace(const Ray& ray) const;
    };
}

#endif //MOBILERAYTRACER_SCENE_HPP