//
// Created by puscas on 16-10-2016.
//

#include "Scene.hpp"
#include "Constants.hpp"

using namespace MobileRT;

Scene::Scene()
{
}

Scene::~Scene()
{
    for (unsigned i = 0; i < this->lights.size(); i++)
    {
        delete this->lights[i];
    }
    
    for (unsigned i = 0; i < this->primitives.size(); i++)
    {
        delete this->primitives[i];
    }
}

Intersection Scene::trace(const Ray &r) const//TODO: utilizar estrutura de aceleraçao
{
    Intersection f_intersection;
    float minT = MAX_LENGTH;
    const unsigned int  n = this->primitives.size();

    for (unsigned int i = 0; i < n; i++)//nao e preciso tar sempre a verificar todas as primitivas
    {
        Intersection intersection(this->primitives[i]->Intersect(r));
        if ((intersection.intersected() == true) && intersection.length() <= minT)//se o que interseta é a primitva mais proxima
        {
            f_intersection = intersection;
            minT = f_intersection.length();
        }
    }
    return f_intersection;//retorna interseçao mais proxima
}

Intersection Scene::shadowTrace(const Ray &ray) const//TODO: utilizar estrutura de aceleraçao
{
    const unsigned int  n = this->primitives.size();
    for (unsigned int i = 0; i < n; i++)//fazer trace do shadow ray
    {
        const Intersection intersection(this->primitives[i]->Intersect(ray));
        if (intersection.intersected() == true)
        {
            return intersection;
        }
    }
    return Intersection();
}