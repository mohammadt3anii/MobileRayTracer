//
// Created by Tiago on 16-10-2016.
//

#include "Scene.h"

using namespace MobileRT;

static unsigned int counter = 0;

Scene::Scene(void) {
    counter++;
}

/*Scene::Scene(const unsigned int sizeLights, Light* lights,
      const unsigned int sizePrimitives, Primitive* primitives) :
        lights_(lights), primitives_(primitives),
        sizeLights_(sizeLights), sizePrimitives_(sizePrimitives)
{
}*/

void Scene::cache() {
    this->sizeLights_ = static_cast<unsigned int> (this->lights_.size());
    this->sizePrimitives_ = static_cast<unsigned int> (this->primitives_.size());
    intersect_ = new std::function<bool(Intersection &, const Ray &)>[sizePrimitives_];
    for (unsigned int i(0); i < sizePrimitives_; i++) {
        intersect_[i] = this->primitives_[static_cast<unsigned long> (i)]->intersect;
    }
}

Scene::~Scene(void) {
    /*const unsigned int sizeLights(static_cast<unsigned int> (this->lights_.size()));
    for (unsigned int i(0); i < sizeLights; i++) {
    delete this->lights_[i];
    }*/
    for (Light *light : this->lights_) {
        delete light;
    }
    this->lights_.clear();

    /*const unsigned int sizePrimitives(static_cast<unsigned int> (this->primitives_.size()));
    for (unsigned int i(0); i < sizePrimitives; i++) {
        delete this->primitives_[i];
    }*/
    for (Primitive *primitive : this->primitives_) {
        delete primitive;
    }
    this->primitives_.clear();
    delete[] intersect_;
}

bool Scene::trace(Intersection &intersection, Ray &ray) const {
    bool res(false);
    ray.maxDistance_ = RAY_LENGTH_MAX;

    for (unsigned int i(0); i < sizePrimitives_; i++) {
        //if (this->primitives_[static_cast<unsigned long> (i)]->intersect(intersection, ray))
        if (intersect_[i](intersection, ray)) {
            ray.maxDistance_ = intersection.length_;
            res = true;
        }
    }
    return res;
}

bool Scene::shadowTrace(Intersection &intersection, const Ray &ray) const {
    for (unsigned int i(0); i < sizePrimitives_; i++)//trace shadow ray
    {
        //if (this->primitives_[i]->intersect(intersection, ray))
        if (intersect_[i](intersection, ray)) {
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
