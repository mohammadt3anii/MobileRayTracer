//
// Created by Tiago on 16-10-2016.
//

#include "Shader.h"

using namespace MobileRT;

Shader::Shader(const Scene &scene) : scene_(scene) {
}

Shader::~Shader(void) {
}

void Shader::shade(RGB &, Intersection &, const Ray &) const {
}

//ray trace and verifies if intersects primitives
void Shader::rayTrace(RGB &rgb, Ray &ray, Intersection &intersection) const {
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

void Shader::setBitmap(unsigned int *bitmap, RGB *accumulate, const float hfov, const float vfov,
                       const unsigned int width, const unsigned int height) {
    this->bitmap_ = bitmap;
    this->accumulate_ = accumulate;
    this->hfov_ = hfov;
    this->vfov_ = vfov;
    this->width_ = width;
    this->height_ = height;
}
