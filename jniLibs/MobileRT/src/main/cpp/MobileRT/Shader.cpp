//
// Created by Tiago on 16-10-2016.
//

#include "Shader.hpp"

using MobileRT::Shader;
using MobileRT::Intersection;

Shader::Shader(Scene &scene, const unsigned int samplesLight) noexcept :
        scene_(scene), samplesLight_(samplesLight) {
}

//ray trace and verifies if intersects primitives
Intersection Shader::rayTrace(RGB &rgb, Ray &&ray) const noexcept {
	Intersection intersection;
	if (this->scene_.trace(intersection, ray) >= 0)
	{
		shade(rgb, intersection, std::move(ray)); // compute radiance
	}
	return intersection;
}

int Shader::traceTouch(Intersection &intersection, const Ray &ray) const noexcept {
    return this->scene_.trace(intersection, ray);
}
