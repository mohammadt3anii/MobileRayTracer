//
// Created by Tiago on 16-10-2016.
//

#include "Scene.hpp"

using ::MobileRT::Scene;
using ::MobileRT::Triangle;
using ::MobileRT::Sphere;
using ::MobileRT::Plane;
using ::MobileRT::Intersection;

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

Intersection Scene::traceLights(Intersection intersection, const Ray ray) const noexcept {
    const unsigned lightsSize{static_cast<unsigned> (lights_.size())};
    for (unsigned i{0}; i < lightsSize; i++) {
        const Light &light{*this->lights_[static_cast<uint32_t> (i)]};
        intersection = light.intersect(intersection, ray);
    }

    return intersection;
}

template<typename T>
Intersection Scene::trace(::std::vector<T> &primitives, Intersection intersection,
                  const Ray ray) noexcept {
    for (T &primitive : primitives) {
        intersection = primitive.intersect(intersection, ray);
    }
    return intersection;
}

Intersection Scene::trace(Intersection intersection, Ray ray) noexcept {
    intersection =
            trace<::MobileRT::Primitive<::MobileRT::Triangle>>(this->triangles_, intersection,
                                                               ray);
    intersection =
            trace<::MobileRT::Primitive<::MobileRT::Sphere>>(this->spheres_, intersection, ray);
    intersection =
            trace<::MobileRT::Primitive<::MobileRT::Plane>>(this->planes_, intersection, ray);
    intersection =
            trace<::MobileRT::Primitive<::MobileRT::Rectangle>>(this->rectangles_, intersection,
                                                                ray);
    intersection = traceLights(intersection, ::std::move(ray));
    return intersection;
}

template<typename T>
Intersection Scene::shadowTrace(::std::vector<T> &primitives, Intersection intersection,
                        const Ray ray) const noexcept {
    for (T &primitive : primitives) {
        const float dist {intersection.length_};
        intersection = primitive.intersect(intersection, ray);
        if (intersection.length_ < dist) {
            return intersection;
        }
    }
    return intersection;
}

Intersection Scene::shadowTrace(Intersection intersection, Ray &&ray) noexcept {
    intersection =
            shadowTrace<::MobileRT::Primitive<Triangle>>(this->triangles_, intersection, ray);
    intersection =
            shadowTrace<::MobileRT::Primitive<Sphere>>(this->spheres_, intersection, ray);
    intersection =
            shadowTrace<::MobileRT::Primitive<Plane>>(this->planes_, intersection, ray);
    intersection =
            shadowTrace<::MobileRT::Primitive<Rectangle>>(this->rectangles_, intersection, ray);
    return intersection;
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
