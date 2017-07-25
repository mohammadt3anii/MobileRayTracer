//
// Created by Tiago on 16-10-2016.
//

#include "Scene.hpp"

using MobileRT::Scene;
using MobileRT::Triangle;
using MobileRT::Sphere;
using MobileRT::Plane;
using MobileRT::Material;

static unsigned int counter(0u);

Scene::Scene() noexcept :
        triangles_(std::vector<Triangle>()),
				spheres_(std::vector<Sphere>()),
				planes_(std::vector<Plane>()),
				materials_(std::vector<Material>()),
        lights_(std::vector<Light *>())
//bb(nullptr)
{
    counter++;
}

Scene::~Scene() noexcept {
    for (Light *light : this->lights_) {
        delete light;
    }
    this->lights_.clear();
}

int Scene::traceLights(Intersection *intersection, const Ray &ray) const noexcept {
    int res(-1);
    const auto lightsSize(static_cast<unsigned int> (lights_.size()));

    for (unsigned int i(0u); i < lightsSize; i++) {
        const Light &light(*this->lights_[static_cast<uint32_t> (i)]);
        if (light.intersect(intersection, ray)) {
            res = static_cast<int> (i);
        }
    }

    return res;
}

int Scene::trace(Intersection *intersection, const Ray &ray) const noexcept {
    int res(-1);
    const auto trianglesSize(static_cast<unsigned int> (triangles_.size()));
    for (unsigned int i(0u); i < trianglesSize; i++) {
        const Triangle &primitive(this->triangles_[static_cast<uint32_t> (i)]);
        if (primitive.intersect(intersection, ray)) {
            res = static_cast<int> (i);
        }
    }

		const auto spheresSize(static_cast<unsigned int> (spheres_.size()));
		for (unsigned int i(0u); i < spheresSize; i++) {
        const Sphere &primitive(this->spheres_[static_cast<uint32_t> (i)]);
        if (primitive.intersect(intersection, ray)) {
            res = static_cast<int> (i + trianglesSize);
        }
    }

		const auto planesSize(static_cast<unsigned int> (planes_.size()));
		for (unsigned int i(0u); i < planesSize; i++) {
        const Plane &primitive(this->planes_[static_cast<uint32_t> (i)]);
        if (primitive.intersect(intersection, ray)) {
            res = static_cast<int> (i + trianglesSize + spheresSize);
        }
    }
		if(res >= 0) {
			intersection->material_ = &materials_[static_cast<uint32_t>(res)];
		}

    traceLights(intersection, ray);

    return res;
}

bool Scene::shadowTrace(Intersection *intersection, Ray &&ray) const noexcept {
    for (const Triangle &primitive : triangles_) {
        if (primitive.intersect(intersection, ray)) {
            return true;
        }
    }

		for (const Sphere &primitive : spheres_) {
        if (primitive.intersect(intersection, ray)) {
            return true;
        }
    }

		for (const Plane &primitive : planes_) {
        if (primitive.intersect(intersection, ray)) {
            return true;
        }
    }

    return false;
}

unsigned int Scene::getInstances() noexcept {
    const unsigned int res(counter);
    counter = 0u;
    return res;
}

void Scene::resetSampling() noexcept {
    for (Light *light : this->lights_) {
        light->resetSampling();
    }
}

void Scene::buildAccelerator() noexcept {
    /*for (Primitive *p : this->primitives_) {
    }*/
}
