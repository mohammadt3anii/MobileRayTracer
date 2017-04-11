//
// Created by Tiago on 16-10-2016.
//

#include "Shader.h"
//#include <cassert>

using namespace MobileRT;

Shader::Shader(Scene &scene, const unsigned int samplesLight) :
        scene_(scene), samplesLight_(samplesLight) {
}

Shader::~Shader(void) {
}

void Shader::shade(RGB &, Intersection &, Ray &) const {
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
