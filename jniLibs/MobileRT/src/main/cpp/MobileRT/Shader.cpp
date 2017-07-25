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
	regularGrid_(Point3D(), Point3D(), nullptr),
	samplesLight_(samplesLight)
{
    this->scene_.triangles_.shrink_to_fit();
		this->scene_.spheres_.shrink_to_fit();
		this->scene_.planes_.shrink_to_fit();
    this->scene_.lights_.shrink_to_fit();
		this->regularGrid_ = RegularGrid(Point3D(-10,-10,-10), Point3D(10,10,10), &scene_);
}

int Shader::traceTouch(Intersection *intersection, const Ray &ray) const noexcept {
    return this->scene_.trace(intersection, ray);
}

Shader::~Shader () noexcept {
}

bool Shader::rayTrace(RGB *rgb, Intersection *intersection, Ray &&ray) const noexcept {
	// if (regularGrid_.intersect(intersection, ray) == true)
	// if (this->scene_.trace(intersection, ray) >= 0)
	if (this->scene_.trace(intersection, ray) >= 0)
	{// compute radiance
		//intersection->material_ = &this->scene_.materials_[static_cast<uint32_t>(5)];
		return shade(rgb, *intersection, std::move(ray)); 
	}
	return false;
}
