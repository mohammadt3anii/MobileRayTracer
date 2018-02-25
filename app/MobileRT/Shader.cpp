//
// Created by Tiago on 16-10-2016.
//

#include "Shader.hpp"
#include "Utils.hpp"
#include <glm/glm.hpp>
#include <utility>

using ::MobileRT::BVH;
using ::MobileRT::Camera;
using ::MobileRT::Intersection;
using ::MobileRT::Ray;
using ::MobileRT::Primitive;
using ::MobileRT::Shader;

Shader::Shader(Scene scene, const uint32_t samplesLight, const Accelerator accelerator) noexcept
        :
        scene_{::std::move(scene)},
        accelerator_{accelerator},
        samplesLight_{samplesLight} {
    this->scene_.triangles_.shrink_to_fit();
    this->scene_.spheres_.shrink_to_fit();
    this->scene_.planes_.shrink_to_fit();
    this->scene_.lights_.shrink_to_fit();
}

void Shader::initializeAccelerators(Camera *const camera) noexcept {
    switch (accelerator_) {
        case Accelerator::NAIVE: {
            break;
        }
        case Accelerator::REGULAR_GRID: {
            ::glm::vec3 min{RayLengthMax, RayLengthMax, RayLengthMax};
            ::glm::vec3 max{-RayLengthMax, -RayLengthMax, -RayLengthMax};
            ::std::vector<Primitive<Triangle> *> triangles{convertVector(this->scene_.triangles_)};
            ::std::vector<Primitive<Sphere> *> spheres{convertVector(this->scene_.spheres_)};
            ::std::vector<Primitive<Plane> *> planes{convertVector(this->scene_.planes_)};
            ::std::vector<Primitive<Rectangle> *> rectangles{convertVector(this->scene_.rectangles_)};
            Scene::getBounds<Primitive<Triangle>>(triangles, &min, &max);
            Scene::getBounds<Primitive<Sphere>>(spheres, &min, &max);
            Scene::getBounds<Primitive<Plane>>(planes, &min, &max);
            Scene::getBounds<Primitive<Rectangle>>(rectangles, &min, &max);
            Scene::getBounds(::std::vector<Camera *> {camera}, &min, &max);
            AABB sceneBounds{min, max};
            regularGrid_ = RegularGrid {sceneBounds, &scene_, 32};
            break;
        }
        case Accelerator::BVH: {
            LOG("PLANES");
            bvhPlanes_ = ::MobileRT::BVH<MobileRT::Plane> {::std::move(scene_.planes_)};
            LOG("RECTANGLES");
            bvhRectangles_ = ::MobileRT::BVH<MobileRT::Rectangle> {::std::move(scene_.rectangles_)};
            LOG("SPHERES");
            bvhSpheres_ = ::MobileRT::BVH<MobileRT::Sphere> {::std::move(scene_.spheres_)};
            LOG("TRIANGLES");
            bvhTriangles_ = ::MobileRT::BVH<MobileRT::Triangle> {::std::move(scene_.triangles_)};
            break;
        }
    }
}

Intersection Shader::traceTouch(Intersection intersection, Ray ray) noexcept {
    return this->scene_.trace(intersection, ::std::move(ray));
}

Shader::~Shader() noexcept {
    LOG("SHADER DELETED");
}

bool Shader::shadowTrace(Intersection intersection, Ray ray) noexcept {
    const float lastDist {intersection.length_};
    switch (accelerator_) {
        case Accelerator::NAIVE: {
            intersection = this->scene_.shadowTrace(intersection, ::std::move(ray));
            break;
        }

        case Accelerator::REGULAR_GRID: {
            intersection = this->regularGrid_.shadowTrace(intersection, ::std::move(ray));
            break;
        }

        case Accelerator::BVH: {
            intersection = this->bvhPlanes_.shadowTrace(intersection, ray);
            intersection = this->bvhRectangles_.shadowTrace(intersection, ray);
            intersection = this->bvhSpheres_.shadowTrace(intersection, ray);
            intersection = this->bvhTriangles_.shadowTrace(intersection, ray);
            break;
        }
    }
    return intersection.length_ < lastDist;
}

bool Shader::rayTrace(::glm::vec3 *rgb, Ray ray) noexcept {
    Intersection intersection {};
    const float lastDist {intersection.length_};
    switch (accelerator_) {
        case Accelerator::NAIVE: {
            intersection = this->scene_.trace(intersection, ray);
            break;
        }

        case Accelerator::REGULAR_GRID: {
            intersection = this->regularGrid_.trace(intersection, ray);
            break;
        }

        case Accelerator::BVH: {
            intersection = this->bvhPlanes_.trace(intersection, ray);
            intersection = this->bvhRectangles_.trace(intersection, ray);
            intersection = this->bvhSpheres_.trace(intersection, ray);
            intersection = this->bvhTriangles_.trace(intersection, ray);
            intersection = this->scene_.traceLights(intersection, ray);
            break;
        }
    }
    return intersection.length_ < lastDist && shade(rgb, intersection, ::std::move(ray));
}
