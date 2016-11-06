//
// Created by puscas on 16-10-2016.
//

#include "Scene.h"
#include "Constants.h"

using namespace MobileRT;

Scene::Scene() {

}

Intersection Scene::trace(const Ray &r) {
    Intersection f_intersection;
    float minT = MAX_LENGTH;
    int  n = primitives.size();

    for (int i = 0; i < n; i++) {
        Intersection intersection(std::move(primitives[i]->Intersect(r)));
        if (intersection.intersected()) {
            if (intersection.length() <= minT) {
                f_intersection = intersection;
                minT = f_intersection.length();
            }
        }
    }
    return f_intersection;
}

Intersection Scene::shadowTrace(const Ray &ray) {
    int  n = primitives.size();
    for (int i = 0; i < n; i++) {
        Intersection intersection(std::move(primitives[i]->Intersect(ray)));
        if (intersection.intersected()) {
            return intersection;
        }
    }
    return Intersection();
}