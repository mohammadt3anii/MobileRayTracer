//
// Created by Tiago on 16-10-2016.
//

#include "Scene.h"

using namespace MobileRT;

static unsigned int counter = 0;

Scene::Scene(void) {
    counter++;
}

Scene::~Scene(void) {
    for (Light *light : this->lights_) {
        delete light;
    }
    this->lights_.clear();
    for (Primitive *primitive : this->primitives_) {
        delete primitive;
    }
    this->primitives_.clear();
}

int Scene::traceLights(Intersection &intersection, Ray &ray) const {
    int res(-1);
    const unsigned int lightsSize(static_cast<unsigned int> (lights_.size()));

    for (unsigned int i(0); i < lightsSize; i++) {
        const Light &light(*this->lights_[static_cast<unsigned long> (i)]);
        if (light.intersect(intersection, ray, light.radiance_))
        {
            ray.maxDistance_ = intersection.length_;
            res = static_cast<int> (i);
        }
    }

    return res;
}

int Scene::trace(Intersection &intersection, Ray &ray) const {
    int res(-1);
    ray.maxDistance_ = RAY_LENGTH_MAX;
    const unsigned int primitivesSize(static_cast<unsigned int> (primitives_.size()));

    for (unsigned int i(0); i < primitivesSize; i++) {
        const Primitive &primitive(*this->primitives_[static_cast<unsigned long> (i)]);
        if (primitive.intersect(intersection, ray))
        {
            ray.maxDistance_ = intersection.length_;
            res = static_cast<int> (i);
        }
    }

    traceLights(intersection, ray);

    return res;
}

bool Scene::shadowTrace(Intersection &intersection, const Ray &ray) const {
    const unsigned int primitivesSize(static_cast<unsigned int> (primitives_.size()));

    for (unsigned int i(0); i < primitivesSize; i++)//trace shadow ray
    {
        const Primitive &primitive(*this->primitives_[static_cast<unsigned long> (i)]);
        if (primitive.intersect(intersection, ray))
        {
            return true;
        }
    }

    return false;
}

unsigned int Scene::getInstances(void) {
    const unsigned int res(counter);
    counter = 0;
    return res;
}

void Scene::resetSampling(void) {
    for (Light *light : this->lights_) {
        light->resetSampling();
    }
}
