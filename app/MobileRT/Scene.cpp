//
// Created by Tiago on 16-10-2016.
//

#include "MobileRT/Scene.hpp"

using ::MobileRT::Scene;
using ::MobileRT::Triangle;
using ::MobileRT::Sphere;
using ::MobileRT::Plane;
using ::MobileRT::Intersection;

Scene::~Scene() noexcept {
    //may not free the memory
    this->triangles_.clear();
    this->spheres_.clear();
    this->planes_.clear();
    this->lights_.clear();

    //force free memory
    ::std::vector<MobileRT::Primitive<MobileRT::Plane>> {}.swap(planes_);
    ::std::vector<MobileRT::Primitive<MobileRT::Sphere>> {}.swap(spheres_);
    ::std::vector<MobileRT::Primitive<MobileRT::Triangle>> {}.swap(triangles_);
    ::std::vector<::std::unique_ptr<Light>> {}.swap(lights_);

    LOG("SCENE DELETED");
}

Intersection Scene::traceLights(Intersection intersection, const Ray &ray) const noexcept {
    const ::std::uint32_t lightsSize {static_cast<::std::uint32_t> (lights_.size())};
    for (::std::uint32_t i {0}; i < lightsSize; ++i) {
        const Light &light{*this->lights_[static_cast<::std::uint32_t> (i)]};
        intersection = light.intersect(intersection, ray);
    }

    return intersection;
}

void Scene::resetSampling() noexcept {
    for (const auto &light : this->lights_) {
        light->resetSampling();
    }
}

void Scene::AABBbounds(const AABB &box, ::glm::vec3 *const min, ::glm::vec3 *const max) {
    *min = ::glm::min(box.pointMin_, *min);

    *max = ::glm::max(box.pointMax_, *max);
}
