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
    this->materials_.clear();

    //force free memory
    ::std::vector<MobileRT::Plane> {}.swap(planes_);
    ::std::vector<MobileRT::Sphere> {}.swap(spheres_);
    ::std::vector<MobileRT::Triangle> {}.swap(triangles_);
    ::std::vector<::std::unique_ptr<Light>> {}.swap(lights_);
    ::std::vector<MobileRT::Material> {}.swap(materials_);

    LOG("SCENE DELETED");
}

bool Scene::traceLights(Intersection *const intersection, const Ray &ray) const noexcept {
    bool intersected {false};
    for (const auto &light : this->lights_) {
        intersected |= light->intersect(intersection, ray);
    }
    return intersected;
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
