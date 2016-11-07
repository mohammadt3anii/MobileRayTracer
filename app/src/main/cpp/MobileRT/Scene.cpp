//
// Created by puscas on 16-10-2016.
//

#include "Scene.h"
#include "Constants.h"

using namespace MobileRT;

Scene::Scene()
{
}

Intersection Scene::trace(const Ray &r)//TODO: utilizar estrutura de aceleraçao
{
    Intersection f_intersection;
    float minT = MAX_LENGTH;
    const int  n = primitives.size();

    for (int i = 0; i < n; i++)//nao e preciso tar sempre a verificar todas as primitivas
    {
        Intersection intersection(primitives[i]->Intersect(r));
        if (intersection.intersected() && intersection.length() <= minT)//se o que interseta é a primitva mais proxima
        {
            f_intersection = intersection;
            minT = f_intersection.length();
        }
    }
    return f_intersection;//retorna interseçao mais proxima
}

Intersection Scene::shadowTrace(const Ray &ray)//TODO: utilizar estrutura de aceleraçao
{
    const int  n = primitives.size();
    for (int i = 0; i < n; i++)//fazer trace do shadow ray
    {
        const Intersection intersection(primitives[i]->Intersect(ray));
        if (intersection.intersected())
        {
            return intersection;
        }
    }
    return Intersection();
}