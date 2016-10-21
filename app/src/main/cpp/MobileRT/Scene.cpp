//
// Created by puscas on 16-10-2016.
//

#include "Scene.h"
#include "Intersection.h"

using namespace MobileRT;

Scene::Scene () {

}

Intersection* Scene::trace (const Ray& r) {
    Intersection* isect = new Intersection();
    Intersection* f_isect = new Intersection();
    float minT = 1E10f;
    int i;
    int  n = primitives.size();

    for (i=0 ; i < n ; i++) {
        isect = primitives[i]->Intersect(r);
        if (isect->intersected) {
            if (isect->t <= minT) {
                f_isect = isect;
                minT = f_isect->t;
            }
        }
    }
    return f_isect;
}

Intersection* Scene::shadowTrace (const Ray& r) {
    Intersection* isect;
    int i;
    int  n = primitives.size();

    for (i=0 ; i < n ; i++) {
        isect = primitives[i]->Intersect(r);
        if (isect->intersected) return isect;
    }
    return new Intersection();
}