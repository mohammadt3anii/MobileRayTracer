//
// Created by Tiago on 16-10-2016.
//

#include "Shader.hpp"

using MobileRT::Intersection;
using MobileRT::Ray;
using MobileRT::RGB;
using MobileRT::Shader;

Shader::Shader(Scene &&scene, const unsigned int samplesLight) noexcept :
        scene_(std::move(scene)),
        samplesLight_(samplesLight)
{
    this->scene_.triangles_.shrink_to_fit();
		this->scene_.spheres_.shrink_to_fit();
		this->scene_.planes_.shrink_to_fit();
    this->scene_.lights_.shrink_to_fit();
}

int Shader::traceTouch(Intersection *intersection, const Ray &ray) const noexcept {
    return this->scene_.trace(intersection, ray);
}

Shader::~Shader () noexcept {
}

bool Shader::rayTrace(RGB *rgb, Intersection *intersection, Ray &&ray) const noexcept {
	if (this->scene_.trace(intersection, ray) >= 0)
	{// compute radiance
		return shade(rgb, *intersection, std::move(ray)); 
	}
	return false;
}
