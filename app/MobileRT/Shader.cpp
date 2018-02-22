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
using ::MobileRT::RGB;
using ::MobileRT::Primitive;
using ::MobileRT::Shader;

Shader::Shader(Scene scene, const unsigned samplesLight, const Accelerator accelerator) noexcept
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
    glm::vec3 min{RayLengthMax, RayLengthMax, RayLengthMax};
    glm::vec3 max{-RayLengthMax, -RayLengthMax, -RayLengthMax};
    ::std::vector<Primitive<Triangle> *> triangles{convertVector(this->scene_.triangles_)};
    ::std::vector<Primitive<Sphere> *> spheres{convertVector(this->scene_.spheres_)};
    ::std::vector<Primitive<Plane> *> planes{convertVector(this->scene_.planes_)};
    ::std::vector<Primitive<Rectangle> *> rectangles{convertVector(this->scene_.rectangles_)};
    Scene::getBounds<Primitive<Triangle>>(triangles, &min, &max);
    Scene::getBounds<Primitive<Sphere>>(spheres, &min, &max);
    Scene::getBounds<Primitive<Plane>>(planes, &min, &max);
    Scene::getBounds<Primitive<Rectangle>>(rectangles, &min, &max);
    Scene::getBounds(::std::vector<Camera *> {camera}, &min, &max);
    AABB sceneBounds{min - Epsilon, max + Epsilon};
    switch (accelerator_) {
        case Accelerator::NAIVE: {
            break;
        }
        case Accelerator::REGULAR_GRID: {
            regularGrid_ = RegularGrid {sceneBounds, &scene_, 32};
            break;
        }
        case Accelerator::BVH: {
            LOG("PLANES");
            bvhPlanes_ = ::MobileRT::BVH<MobileRT::Plane> {std::move(scene_.planes_)};
            LOG("RECTANGLES");
            bvhRectangles_ = ::MobileRT::BVH<MobileRT::Rectangle> {std::move(scene_.rectangles_)};
            LOG("SPHERES");
            bvhSpheres_ = ::MobileRT::BVH<MobileRT::Sphere> {std::move(scene_.spheres_)};
            LOG("TRIANGLES");
            bvhTriangles_ = ::MobileRT::BVH<MobileRT::Triangle> {std::move(scene_.triangles_)};
            break;
        }

        case Accelerator::BVH_vector: {
            LOG("PLANES");
            bvhPlanes2_ = ::MobileRT::BVH_vector<MobileRT::Plane> {std::move(scene_.planes_)};
            LOG("RECTANGLES");
            bvhRectangles2_ = ::MobileRT::BVH_vector<MobileRT::Rectangle> {std::move(scene_.rectangles_)};
            LOG("SPHERES");
            bvhSpheres2_ = ::MobileRT::BVH_vector<MobileRT::Sphere> {std::move(scene_.spheres_)};
            LOG("TRIANGLES");
            bvhTriangles2_ = ::MobileRT::BVH_vector<MobileRT::Triangle> {std::move(scene_.triangles_)};
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
    const float dist {intersection.length_};
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
        case Accelerator::BVH_vector: {
            intersection = this->bvhPlanes2_.shadowTrace(intersection, ray);
            intersection = this->bvhRectangles2_.shadowTrace(intersection, ray);
            intersection = this->bvhSpheres2_.shadowTrace(intersection, ray);
            intersection = this->bvhTriangles2_.shadowTrace(intersection, ray);
            break;
        }
    }
    return intersection.length_ < dist;
}

bool Shader::rayTrace(RGB *rgb, Ray ray) noexcept {
    Intersection intersection {};
    const float dist {intersection.length_};
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
        case Accelerator::BVH_vector: {
            intersection = this->bvhPlanes2_.trace(intersection, ray);
            intersection = this->bvhRectangles2_.trace(intersection, ray);
            intersection = this->bvhSpheres2_.trace(intersection, ray);
            intersection = this->bvhTriangles2_.trace(intersection, ray);
            intersection = this->scene_.traceLights(intersection, ray);
            break;
        }
    }
    return intersection.length_ < dist && shade(rgb, intersection, ::std::move(ray));
}
