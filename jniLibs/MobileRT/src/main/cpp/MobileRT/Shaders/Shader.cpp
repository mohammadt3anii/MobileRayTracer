//
// Created by Tiago on 16-10-2016.
//

#include "Shader.h"
#include <cassert>

using namespace MobileRT;

Shader::Shader(const Scene &scene) : scene_(scene) {
}

Shader::~Shader(void) {
}

void Shader::shade(RGB &, Intersection &, Ray &) const {
}

//ray trace and verifies if intersects primitives
void Shader::rayTrace(RGB &rgb, Ray &ray, Intersection intersection) const {
    assert(!std::isnan(intersection.point_.x_));
    assert(!std::isnan(intersection.point_.y_));
    assert(!std::isnan(intersection.point_.z_));
    assert(!std::isnan(intersection.normal_.x_));
    assert(!std::isnan(intersection.normal_.y_));
    assert(!std::isnan(intersection.normal_.z_));
    assert(!std::isnan(intersection.length_));
    assert(!std::isnan(intersection.symNormal_.x_));
    assert(!std::isnan(intersection.symNormal_.y_));
    assert(!std::isnan(intersection.symNormal_.z_));
    assert(!std::isnan(ray.depth_));
    assert(!std::isnan(ray.direction_.x_));
    assert(!std::isnan(ray.direction_.y_));
    assert(!std::isnan(ray.direction_.z_));
    assert(!std::isnan(ray.maxDistance_));
    assert(!std::isnan(ray.origin_.x_));
    assert(!std::isnan(ray.origin_.y_));
    assert(!std::isnan(ray.origin_.z_));
    assert(!std::isnan(ray.symDirection_.x_));
    assert(!std::isnan(ray.symDirection_.y_));
    assert(!std::isnan(ray.symDirection_.z_));
    if (this->scene_.trace(intersection, ray) >= 0) {
        rgb.reset();
        shade(rgb, intersection, ray);// compute radiance
    }
    else {
        rgb.reset(0.0f);//pixel color without intersection
    }
}

int Shader::traceTouch(Intersection &intersection, Ray &ray) const {
    return this->scene_.trace(intersection, ray);
}
