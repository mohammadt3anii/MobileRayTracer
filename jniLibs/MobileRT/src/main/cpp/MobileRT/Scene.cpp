//
// Created by Tiago on 16-10-2016.
//

#include "Scene.h"

using namespace MobileRT;

static unsigned int counter(0u);

Scene::Scene(void) noexcept {
    counter++;
}

Scene::~Scene(void) noexcept {
    for (Light *light : this->lights_) {
        delete light;
    }
    this->lights_.clear();
    for (Primitive *primitive : this->primitives_) {
        delete primitive;
    }
    this->primitives_.clear();
}

int Scene::traceLights(Intersection &intersection, const Ray &ray) const noexcept {
    int res(-1);
    const unsigned int lightsSize(static_cast<unsigned int> (lights_.size()));

    for (unsigned int i(0u); i < lightsSize; i++) {
        const Light &light(*this->lights_[static_cast<unsigned long> (i)]);
        if (light.intersectL(intersection, ray)) {
            res = static_cast<int> (i);
        }
    }

    return res;
}

int Scene::trace(Intersection &intersection, const Ray &ray) const noexcept {
    int res(-1);
    const unsigned int primitivesSize(static_cast<unsigned int> (primitives_.size()));

    for (unsigned int i(0u); i < primitivesSize; i++) {
        const Primitive &primitive(*this->primitives_[static_cast<unsigned long> (i)]);
        if (primitive.intersect(intersection, ray)) {
            res = static_cast<int> (i);
        }
    }

    traceLights(intersection, ray);

    return res;
}

bool Scene::shadowTrace(Intersection &intersection, const Ray &ray) const noexcept {
    for (const Primitive *primitive : this->primitives_)//trace shadow ray
    {
        if (primitive->intersect(intersection, ray)) {
            return true;
        }
    }

    return false;
}

unsigned int Scene::getInstances(void) noexcept {
    const unsigned int res(counter);
    counter = 0u;
    return res;
}

void Scene::resetSampling(void) noexcept {
    for (Light *light : this->lights_) {
        light->resetSampling();
    }
}

void Scene::buildAccelerator(void) noexcept {
    for (Primitive *p : this->primitives_) {
    }
}
