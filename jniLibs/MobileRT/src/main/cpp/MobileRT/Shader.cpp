//
// Created by Tiago on 16-10-2016.
//

#include "Shader.hpp"

using MobileRT::Shader;
using MobileRT::Intersection;

Shader::Shader(Scene &&scene, const unsigned int samplesLight) noexcept :
        scene_(scene), samplesLight_(samplesLight) {
}

int Shader::traceTouch(Intersection &intersection, const Ray &ray) const noexcept {
    return this->scene_.trace(intersection, ray);
}
