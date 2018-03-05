//
// Created by Tiago on 16-10-2016.
//

#include "MobileRT/Shader.hpp"
#include "MobileRT/Utils.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <utility>

using ::MobileRT::BVH;
using ::MobileRT::Camera;
using ::MobileRT::Intersection;
using ::MobileRT::Ray;
using ::MobileRT::Primitive;
using ::MobileRT::Shader;

namespace {
    const ::std::uint32_t MASK{0xFFFFF};
    const ::std::uint32_t SIZE{MASK + 1};
    ::std::array<float, SIZE> VALUES{};

    bool FillThings() {
        static ::std::mt19937 generator(::std::random_device{}());
        for (::std::uint32_t i{0}; i < SIZE; ++i) {
            VALUES.at(i) = ::MobileRT::haltonSequence(i, 2);
        }
        ::std::shuffle(VALUES.begin(), VALUES.end(), generator);
        return true;
    }
}//namespace

Shader::Shader(
    Scene scene,
    const ::std::uint32_t samplesLight,
    const Accelerator accelerator) noexcept :
        scene_{::std::move(scene)},
        accelerator_{accelerator},
        samplesLight_{samplesLight}
{
    static bool unused{FillThings()};
    static_cast<void> (unused);
}

void Shader::initializeAccelerators(Camera *const camera) noexcept {
    switch (accelerator_) {
        case Accelerator::NAIVE: {
            break;
        }
        case Accelerator::REGULAR_GRID: {
            ::glm::vec3 min {RayLengthMax};
            ::glm::vec3 max {-RayLengthMax};
            ::std::vector<Primitive<Triangle> *> triangles{convertVector(this->scene_.triangles_)};
            ::std::vector<Primitive<Sphere> *> spheres{convertVector(this->scene_.spheres_)};
            ::std::vector<Primitive<Plane> *> planes{convertVector(this->scene_.planes_)};
            Scene::getBounds<Primitive<Triangle>>(triangles, &min, &max);
            Scene::getBounds<Primitive<Sphere>>(spheres, &min, &max);
            Scene::getBounds<Primitive<Plane>>(planes, &min, &max);
            Scene::getBounds(::std::vector<Camera *> {camera}, &min, &max);
            AABB sceneBounds{min, max};
            regularGrid_ = RegularGrid {sceneBounds, &scene_, 32};
            break;
        }
        case Accelerator::BVH: {
            bvhPlanes_ = ::MobileRT::BVH<MobileRT::Plane> {::std::move(scene_.planes_)};
            bvhSpheres_ = ::MobileRT::BVH<MobileRT::Sphere> {::std::move(scene_.spheres_)};
            bvhTriangles_ = ::MobileRT::BVH<MobileRT::Triangle> {::std::move(scene_.triangles_)};
            break;
        }
    }
}

Intersection Shader::traceTouch(Intersection intersection, const Ray &ray) noexcept {
    return this->scene_.trace(intersection, ray);
}

Shader::~Shader() noexcept {
    LOG("SHADER DELETED");
}

bool Shader::shadowTrace(Intersection intersection, const Ray &ray) noexcept {
    const float lastDist {intersection.length_};
    switch (accelerator_) {
        case Accelerator::NAIVE: {
            intersection = this->scene_.shadowTrace(intersection, ray);
            break;
        }

        case Accelerator::REGULAR_GRID: {
            intersection = this->regularGrid_.shadowTrace(intersection, ray);
            break;
        }

        case Accelerator::BVH: {
            intersection = this->bvhPlanes_.shadowTrace(intersection, ray);
            intersection = this->bvhSpheres_.shadowTrace(intersection, ray);
            intersection = this->bvhTriangles_.shadowTrace(intersection, ray);
            break;
        }
    }
    return intersection.length_ < lastDist;
}

bool Shader::rayTrace(::glm::vec3 *rgb, const Ray &ray) noexcept {
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
            intersection = this->bvhSpheres_.trace(intersection, ray);
            intersection = this->bvhTriangles_.trace(intersection, ray);
            intersection = this->scene_.traceLights(intersection, ray);
            break;
        }
    }
    return intersection.length_ < lastDist && shade(rgb, intersection, ray);
}

void Shader::resetSampling() noexcept {
    this->scene_.resetSampling();
}

::glm::vec3 Shader::getCosineSampleHemisphere(const ::glm::vec3 &normal) const noexcept {
    static ::std::atomic<::std::uint32_t> sampler {0};
    const ::std::uint32_t current1 {sampler.fetch_add(1, ::std::memory_order_relaxed)};
    const ::std::uint32_t current2 {sampler.fetch_add(1, ::std::memory_order_relaxed)};
    const float sample1 {VALUES.at(current1 & MASK)};
    const float sample2 {VALUES.at(current2 & MASK)};

    const float r1 {::glm::two_pi<float>() * sample1};
    const float r2 {sample2};
    const float r2s {::std::sqrt(r2)};
    ::glm::vec3 u {::std::abs(normal[0]) > 0.1f ?
               ::glm::cross(::glm::vec3 {0.0f, 1.0f, 0.0f}, normal) :
               ::glm::cross(::glm::vec3 {1.0f, 0.0f, 0.0f}, normal)};
    u = ::glm::normalize(u);
    const ::glm::vec3 &v {::glm::cross(normal, u)};
    ::glm::vec3 direction {(u * (::std::cos(r1) * r2s) +
                        v * (::std::sin(r1) * r2s) +
                        normal * ::std::sqrt(1.0f - r2))};
    direction = ::glm::normalize(direction);
    return direction;
}

::std::uint32_t Shader::getLightIndex () {
    static ::std::atomic<::std::uint32_t> sampler {0};
    const ::std::uint32_t current {sampler.fetch_add(1, ::std::memory_order_relaxed)};
    const float sample {VALUES.at(current & MASK)};

    const ::std::uint32_t sizeLights {static_cast<::std::uint32_t>(scene_.lights_.size())};
    const float randomNumber {sample};
    const ::std::uint32_t chosenLight {
        static_cast<::std::uint32_t> (::std::floor(randomNumber * sizeLights * 0.99999f))};
    return chosenLight;
}
