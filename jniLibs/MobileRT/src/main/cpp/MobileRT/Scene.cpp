//
// Created by Tiago on 16-10-2016.
//

#include "Scene.h"

using namespace MobileRT;

Scene::Scene() {
}

Scene::~Scene() {
    const unsigned int sizeLights(static_cast<unsigned int> (this->lights_.size()));
    for (unsigned int i(0); i < sizeLights; i++) {
        delete this->lights_[i];
    }
    const unsigned int sizePrimitives(static_cast<unsigned int> (this->primitives_.size()));
    for (unsigned int i(0); i < sizePrimitives; i++) {
        delete this->primitives_[i];
    }
}

int Scene::trace(Intersection &intersection, Ray &ray) const {
    int value(-1);
    ray.maxDistance_ = RAY_LENGTH_MAX;
    const int sizePrimitives(static_cast<int> (this->primitives_.size()));

    for (int i(0); i < sizePrimitives; i++) {
        if (this->primitives_[static_cast<unsigned long> (i)]->intersect(intersection, ray)) {
            ray.maxDistance_ = intersection.length_;
            value = i;
        }
    }
    return value;
}

bool Scene::shadowTrace(Intersection &intersection, const Ray &ray) const {
    const unsigned int sizePrimitives(static_cast<unsigned int> (this->primitives_.size()));
    for (unsigned int i(0); i < sizePrimitives; i++)//trace shadow ray
    {
        if (this->primitives_[i]->intersect(intersection, ray)) {
            return true;
        }
    }
    return false;
}
