//
// Created by Tiago on 16-10-2016.
//

#include "Scene.h"
#include "../Constants.h"

using namespace MobileRT;

Scene::Scene()
{
}

Scene::~Scene()
{
    const unsigned int sizeLights (this->lights.size());
    for (unsigned int i (0); i < sizeLights; i++)
    {
        delete this->lights[i];
    }
    const unsigned int sizePrimitives (this->primitives.size());
    for (unsigned int i (0); i < sizePrimitives; i++)
    {
        delete this->primitives[i];
    }
}

bool Scene::trace(Intersection& intersection, Ray& ray) const//TODO: utilizar estrutura de aceleraçao
{
    bool value (false);
    ray.maxDistance_ = RAY_LENGTH_MAX;
    const unsigned int sizePrimitives (this->primitives.size());

    for (unsigned int i (0); i < sizePrimitives; i++)//nao e preciso tar sempre a verificar todas as primitivas
    {
        if (this->primitives[i]->intersect(intersection, ray) == true)
        {
            ray.maxDistance_ = intersection.length_;
            value = true;
        }
    }
    return value;
}

bool Scene::shadowTrace(Intersection& intersection, const Ray& ray) const//TODO: utilizar estrutura de aceleraçao
{
    const unsigned int sizePrimitives (this->primitives.size());
    for (unsigned int i (0); i < sizePrimitives; i++)//fazer trace do shadow ray
    {
        if (this->primitives[i]->intersect(intersection, ray) == true)
        {
            return true;
        }
    }
    return false;
}