//
// Created by Tiago on 16-10-2016.
//

#include "Scene.h"

using namespace MobileRT;

Scene::Scene() {
}

Scene::~Scene() {
    const unsigned int sizeLights(static_cast<unsigned int> (this->lights.size()));
    for (unsigned int i(0); i < sizeLights; i++) {
        delete this->lights[i];
    }
    const unsigned int sizePrimitives(static_cast<unsigned int> (this->primitives.size()));
    for (unsigned int i(0); i < sizePrimitives; i++) {
        delete this->primitives[i];
    }
}

bool Scene::trace(Intersection &intersection, Ray &ray) const {
    bool value(false);
    ray.maxDistance_ = RAY_LENGTH_MAX;
    const unsigned int sizePrimitives(static_cast<unsigned int> (this->primitives.size()));

    for (unsigned int i(0); i < sizePrimitives; i++) {
        if (this->primitives[i]->intersect(intersection, ray)) {
            ray.maxDistance_ = intersection.length_;
            value = true;
        }
    }
    return value;
}

bool Scene::shadowTrace(Intersection &intersection, const Ray &ray) const {
    const unsigned int sizePrimitives(static_cast<unsigned int> (this->primitives.size()));
    for (unsigned int i(0); i < sizePrimitives; i++)//trace shadow ray
    {
        if (this->primitives[i]->intersect(intersection, ray)) {
            return true;
        }
    }
    return false;
}