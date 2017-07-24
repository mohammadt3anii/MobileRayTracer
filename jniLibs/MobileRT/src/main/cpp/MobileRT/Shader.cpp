//
// Created by Tiago on 16-10-2016.
//

#include "Shader.hpp"

using MobileRT::Shader;
using MobileRT::Intersection;

Shader::Shader(Scene &&scene, const unsigned int samplesLight) noexcept :
        scene_(std::move(scene)),
        samplesLight_(samplesLight)
{
    this->scene_.primitives_.shrink_to_fit();
    this->scene_.lights_.shrink_to_fit();
}

int Shader::traceTouch(Intersection &intersection, const Ray &ray) const noexcept {
    return this->scene_.trace(intersection, ray);
}

Shader::~Shader () noexcept {
}
