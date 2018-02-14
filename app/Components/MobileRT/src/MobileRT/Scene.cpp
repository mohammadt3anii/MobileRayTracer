//
// Created by Tiago on 16-10-2016.
//

#include "Scene.hpp"

using ::MobileRT::Scene;
using ::MobileRT::Triangle;
using ::MobileRT::Sphere;
using ::MobileRT::Plane;
static unsigned counter{0};

Scene::~Scene() noexcept {
    //may not free the memory
    this->triangles_.clear();
    this->spheres_.clear();
    this->planes_.clear();
    this->rectangles_.clear();
    this->lights_.clear();

    //force free memory
    std::vector<MobileRT::Primitive<MobileRT::Plane>>().swap(planes_);
    std::vector<MobileRT::Primitive<MobileRT::Rectangle>>().swap(rectangles_);
    std::vector<MobileRT::Primitive<MobileRT::Sphere>>().swap(spheres_);
    std::vector<MobileRT::Primitive<MobileRT::Triangle>>().swap(triangles_);
    std::vector<::std::unique_ptr<Light>>().swap(lights_);

    LOG("SCENE DELETED");
}

bool Scene::traceLights(Intersection *const intersection, const Ray ray) const noexcept {
    bool res{false};
    const unsigned lightsSize{static_cast<unsigned> (lights_.size())};
    for (unsigned i{0}; i < lightsSize; i++) {
        const Light &light{*this->lights_[static_cast<uint32_t> (i)]};
        if (light.intersect(intersection, ray)) {
            res = true;
        }
    }

    return res;
}

template<typename T>
bool Scene::trace(::std::vector<T> &primitives, Intersection *const intersection,
                  const Ray ray) noexcept {
    bool res{false};
    for (T &primitive : primitives) {
        res |= primitive.intersect(intersection, ray);
    }
    return res;
}

bool Scene::trace(Intersection *const intersection, Ray ray) noexcept {
    bool intersectedTriangles{
            trace<::MobileRT::Primitive<::MobileRT::Triangle>>(this->triangles_, intersection,
                                                               ray)};
    bool intersectedSpheres{
            trace<::MobileRT::Primitive<::MobileRT::Sphere>>(this->spheres_, intersection, ray)};
    bool intersectedPlanes{
            trace<::MobileRT::Primitive<::MobileRT::Plane>>(this->planes_, intersection, ray)};
    bool intersectedRectangles{
            trace<::MobileRT::Primitive<::MobileRT::Rectangle>>(this->rectangles_, intersection,
                                                                ray)};
    bool intersectedLights{
            traceLights(intersection, ::std::move(ray))
    };
    return intersectedTriangles || intersectedSpheres || intersectedPlanes ||
           intersectedRectangles ||
           intersectedLights;
}

template<typename T>
bool Scene::shadowTrace(::std::vector<T> &primitives, Intersection *const intersection,
                        const Ray ray) const noexcept {
    for (T &primitive : primitives) {
        if (primitive.intersect(intersection, ray)) {
            return true;
        }
    }
    return false;
}

bool Scene::shadowTrace(Intersection *const intersection, Ray &&ray) noexcept {
    const bool intersectedTriangles{
            shadowTrace<::MobileRT::Primitive<Triangle>>(this->triangles_, intersection, ray)};
    const bool intersectedSpheres{
            shadowTrace<::MobileRT::Primitive<Sphere>>(this->spheres_, intersection, ray)};
    const bool intersectedPlanes{
            shadowTrace<::MobileRT::Primitive<Plane>>(this->planes_, intersection, ray)};
    const bool intersectedRectangles{
            shadowTrace<::MobileRT::Primitive<Rectangle>>(this->rectangles_, intersection, ray)};
    return intersectedTriangles || intersectedSpheres || intersectedPlanes || intersectedRectangles;
}

unsigned Scene::getInstances() noexcept {
    const unsigned res{counter};
    counter = 0;
    return res;
}

void Scene::resetSampling() noexcept {
    for (::std::unique_ptr<Light> &light : this->lights_) {
        light->resetSampling();
    }
}

void Scene::AABBbounds(const AABB box, Point3D *const min, Point3D *const max) {
    min->setX(::std::min(box.pointMin_.x_(), min->x_()));
    min->setY(::std::min(box.pointMin_.y_(), min->y_()));
    min->setZ(::std::min(box.pointMin_.z_(), min->z_()));

    max->setX(::std::max(box.pointMax_.x_(), max->x_()));
    max->setY(::std::max(box.pointMax_.y_(), max->y_()));
    max->setZ(::std::max(box.pointMax_.z_(), max->z_()));
}
