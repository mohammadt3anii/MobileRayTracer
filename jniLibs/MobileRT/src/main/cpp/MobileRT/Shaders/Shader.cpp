//
// Created by Tiago on 16-10-2016.
//

#include "Shader.h"

using namespace MobileRT;

Shader::Shader(const Scene &scene) : scene_(scene) {
}

Shader::~Shader() {
}

void Shader::shade(RGB &, Intersection &, const Ray &, Vector3D &) const {
}

//ray trace and verifies if intersects primitives
void Shader::rayTrace(RGB &rgb, Ray &ray, Intersection &intersection, Vector3D &vector) const {
    if (this->scene_.trace(intersection, ray) >= 0) {
        shade(rgb, intersection, ray, vector);// compute radiance
    }
    else {
        rgb.reset(0.0f, 0.0f, 0.0f);//pixel color without intersection
    }
}

int Shader::traceTouch(Intersection &intersection, Ray &ray) const {
    return this->scene_.trace(intersection, ray);
}