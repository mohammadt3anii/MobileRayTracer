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

const Intersection* Scene::trace(const Ray& r) const//TODO: utilizar estrutura de aceleraçao
{
    const Intersection* f_intersection = nullptr;
    const Intersection* intersection = nullptr;
    float minT = MAX_LENGTH;
    const unsigned int n = this->primitives.size();

    for (unsigned int i = 0; i < n; i++)//nao e preciso tar sempre a verificar todas as primitivas
    {
        intersection = this->primitives[i]->Intersect(r, minT);
        if ((intersection != nullptr))//se o que interseta é a primitva mais proxima
        {
            f_intersection = intersection;
            minT = f_intersection->length();
        } else
        {
            delete intersection;
        }
    }
    return f_intersection;//retorna interseçao mais proxima
}

const Intersection* Scene::shadowTrace(const Ray& ray) const//TODO: utilizar estrutura de aceleraçao
{
    const Intersection* intersection = nullptr;
    const unsigned int  n = this->primitives.size();
    for (unsigned int i = 0; i < n; i++)//fazer trace do shadow ray
    {
        //const Intersection intersection(this->primitives[i]->Intersect(ray));
        intersection = this->primitives[i]->Intersect(ray, MAX_LENGTH);
        if (intersection != nullptr)
        {
            return intersection;
        }
    }
    //return Intersection();
    return nullptr;
}