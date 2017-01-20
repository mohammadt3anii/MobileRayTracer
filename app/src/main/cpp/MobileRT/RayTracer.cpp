//
// Created by Tiago on 16-10-2016.
//

#include "RayTracer.h"

using namespace MobileRT;

RayTracer::RayTracer(const Scene &scene, const Shader &shader) :
        scene_(scene),
        shader_(shader),
        rayTraceCall_([this](RGB &rgb, Ray &ray, Intersection &intersection, Vector3D &vector) {
        this->rayTrace(rgb, ray, intersection, vector);
        }) {
}

RayTracer::~RayTracer() {
}

//ray trace and verifies if intersects primitives
void RayTracer::rayTrace(RGB &rgb, Ray &ray, Intersection &intersection, Vector3D &vector) const {
    // compute radiance
    if (this->scene_.trace(intersection, ray) >= 0)
    {
        this->shader_.shade(rgb, intersection, ray, vector, this->rayTraceCall_);
    }
    else {
        rgb.reset(0.0f, 0.0f, 0.0f);//pixel color without intersection
    }
}

int RayTracer::traceTouch(Ray &ray, Intersection &intersection) const {
    // compute radiance
    const int primitiveID = this->scene_.trace(intersection, ray);
    return primitiveID;
}