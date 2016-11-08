//
// Created by puscas on 16-10-2016.
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

            Intersection trace(const Ray& ray) const;

            Intersection shadowTrace(const Ray& ray) const;
    };
}

#endif //MOBILERAYTRACER_SCENE_H