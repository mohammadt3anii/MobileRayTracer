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

int Scene::trace(Intersection &intersection, Ray &ray) const {
    int res(-1);
    ray.maxDistance_ = RAY_LENGTH_MAX;

    for (unsigned int i(0); i < primitives_.size(); i++) {
        if (this->primitives_[static_cast<unsigned long> (i)]->intersect(intersection, ray))
        {
            ray.maxDistance_ = intersection.length_;
            res = static_cast<int> (i);
        }
    }
    return res;
}

bool Scene::shadowTrace(Intersection &intersection, const Ray &ray) const {
    for (unsigned int i(0); i < primitives_.size(); i++)//trace shadow ray
    {
        if (this->primitives_[i]->intersect(intersection, ray))
        {
            return true;
        }
    }
    return false;
}

unsigned int Scene::getInstances() {
    unsigned int res(counter);
    counter = 0;
    return res;
}
