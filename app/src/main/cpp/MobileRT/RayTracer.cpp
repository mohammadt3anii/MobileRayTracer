//
// Created by Tiago on 16-10-2016.
//

#include "RayTracer.h"
#include "Shaders/NoShadows.h"
#include "Shaders/Whitted.h"

using namespace MobileRT;

RayTracer::RayTracer(const Scene &scene, const unsigned int shader) :
        scene_(scene) {
    switch (shader) {
        case 0:
            this->shader_ = new NoShadows(scene);
            break;

        case 1:
            this->shader_ = new Whitted(scene);
            break;

        default:
            this->shader_ = nullptr;
            break;
    }

    this->rayTraceCall_ = [this](RGB &rgb, Ray &ray, Intersection &intersection, Vector3D &vector) {
        this->rayTrace(rgb, ray, intersection, vector);
    };
}

RayTracer::~RayTracer() {
    delete this->shader_;
}

//ray trace and verifies if intersects primitives
void RayTracer::rayTrace(RGB &rgb, Ray &ray, Intersection &intersection, Vector3D &vector) const {
    // compute radiance
    if (this->scene_.trace(intersection, ray))
    {
        this->shader_->shade(rgb, intersection, ray, vector, this->rayTraceCall_);
    }
    else {
        rgb.reset(0.0f, 0.0f, 0.0f);//pixel color without intersection
    }
}