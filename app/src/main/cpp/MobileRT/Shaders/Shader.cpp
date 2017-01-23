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

void Shader::setRayTraceFunction(const RayTraceCall &rayTraceCall) {
    this->rayTraceCall_ = rayTraceCall;
}