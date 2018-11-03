//
// Created by Tiago on 16-10-2016.
//

#include "MobileRT/Shader.hpp"
#include "MobileRT/Utils.hpp"
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <random>
#include <utility>

using ::MobileRT::Camera;
using ::MobileRT::Intersection;
using ::MobileRT::Ray;
using ::MobileRT::Shader;

Shader::Shader(
    Scene &&scene,
    const ::std::uint32_t samplesLight,
    const Accelerator accelerator) noexcept :
        scene_{::std::move(scene)},
        accelerator_{accelerator},
        samplesLight_{samplesLight} {
}

Shader::~Shader() noexcept {
    LOG("SHADER DELETED");
}

void Shader::initializeAccelerators(Camera *const camera) noexcept {
    switch (accelerator_) {
        case Accelerator::NAIVE: {
            naivePlanes_ = ::MobileRT::Naive {::std::move(scene_.planes_)};
            naiveSpheres_ = ::MobileRT::Naive {::std::move(scene_.spheres_)};
            naiveTriangles_ = ::MobileRT::Naive {::std::move(scene_.triangles_)};
            break;
        }

        case Accelerator::REGULAR_GRID: {
            ::std::vector<Plane*> planes{convertVector(this->scene_.planes_)};
            ::std::vector<Sphere*> spheres{convertVector(this->scene_.spheres_)};
            ::std::vector<Triangle*> triangles{convertVector(this->scene_.triangles_)};

            ::glm::vec3 minPlanes {RayLengthMax};
            ::glm::vec3 maxPlanes {-RayLengthMax};
            ::glm::vec3 minSpheres {RayLengthMax};
            ::glm::vec3 maxSpheres {-RayLengthMax};
            ::glm::vec3 minTriangles {RayLengthMax};
            ::glm::vec3 maxTriangles {-RayLengthMax};

            Scene::getBounds(planes, &minPlanes, &maxPlanes);
            Scene::getBounds(spheres, &minSpheres, &maxSpheres);
            Scene::getBounds(triangles, &minTriangles, &maxTriangles);
            
            Scene::getBounds(::std::vector<Camera*> {camera}, &minPlanes, &maxPlanes);
            Scene::getBounds(::std::vector<Camera*> {camera}, &minSpheres, &maxSpheres);
            Scene::getBounds(::std::vector<Camera*> {camera}, &minTriangles, &maxTriangles);

            const AABB sceneBoundsPlanes {minPlanes - Epsilon, maxPlanes + Epsilon};
            const AABB sceneBoundsSpheres {minSpheres - Epsilon, maxSpheres + Epsilon};
            const AABB sceneBoundsTriangles {minTriangles - Epsilon, maxTriangles + Epsilon};

            const ::std::uint32_t sizePlanes{static_cast<::std::uint32_t> (planes.size())};
            const ::std::uint32_t sizeSpheres{static_cast<::std::uint32_t> (spheres.size())};
            const ::std::uint32_t sizeTriangles{static_cast<::std::uint32_t> (triangles.size())};
            LOG("sizePlanes = ", sizePlanes);
            LOG("sizeSpheres = ", sizeSpheres);
            LOG("sizeTriangles = ", sizeTriangles);
            /*const ::std::int32_t auxPlanes {::std::max(2, static_cast<::std::int32_t> (roundUpToPowerOf2(
                                                   bitCounter(sizePlanes))))};
            const ::std::int32_t auxSpheres {::std::max(2, static_cast<::std::int32_t> (roundUpToPowerOf2(
                                                   bitCounter(sizeSpheres))))};
            const ::std::int32_t auxTriangles {::std::max(2, static_cast<::std::int32_t> (roundUpToPowerOf2(
                                                   bitCounter(sizeTriangles))))};*/
            const ::std::int32_t auxPlanes{sizePlanes > 6 ?
                                           static_cast<::std::int32_t> (roundUpToPowerOf2(
                                                   bitCounter(sizePlanes))) : 2};
            const ::std::int32_t auxSpheres{sizeSpheres > 6 ?
                                            static_cast<::std::int32_t> (roundUpToPowerOf2(
                                                    bitCounter(sizeSpheres))) : 2};
            const ::std::int32_t auxTriangles{sizeTriangles > 6 ?
                                              static_cast<::std::int32_t> (roundUpToPowerOf2(
                                                      bitCounter(sizeTriangles))) : 2};
            LOG("auxPlanes = ", auxPlanes);
            LOG("auxSpheres = ", auxSpheres);
            LOG("auxTriangles = ", auxTriangles);
            const ::std::int32_t auxPlanes2{static_cast<::std::int32_t> (
                                                    roundUpToPowerOf2(bitCounter(
                                                            static_cast<::std::uint32_t> (auxPlanes))) /
                                                    2)};
            const ::std::int32_t auxSpheres2{static_cast<::std::int32_t> (
                                                     roundUpToPowerOf2(bitCounter(
                                                             static_cast<::std::uint32_t> (auxSpheres))) /
                                                     2)};
            const ::std::int32_t auxTriangles2{static_cast<::std::int32_t> (
                                                       roundUpToPowerOf2(bitCounter(
                                                               static_cast<::std::uint32_t> (auxTriangles))) /
                                                       2)};
            LOG("auxPlanes2 = ", auxPlanes2);
            LOG("auxSpheres2 = ", auxSpheres2);
            LOG("auxTriangles2 = ", auxTriangles2);
            const ::std::int32_t gridSizePlanes {::std::max(auxPlanes * auxPlanes2, auxPlanes)};
            const ::std::int32_t gridSizeSpheres {::std::max(auxSpheres * auxSpheres2, auxSpheres)};
            const ::std::int32_t gridSizeTriangles {::std::max(auxTriangles * auxTriangles2, auxTriangles)};
            LOG("gridSizePlanes = ", gridSizePlanes);
            LOG("gridSizeSpheres = ", gridSizeSpheres);
            LOG("gridSizeTriangles = ", gridSizeTriangles);
            regularGridPlanes_ = ::MobileRT::RegularGrid {sceneBoundsPlanes, ::std::move(scene_.planes_), gridSizePlanes};
            regularGridSpheres_ = ::MobileRT::RegularGrid {sceneBoundsSpheres, ::std::move(scene_.spheres_), gridSizeSpheres};
            regularGridTriangles_ = ::MobileRT::RegularGrid {sceneBoundsTriangles, ::std::move(scene_.triangles_), gridSizeTriangles};
            break;
        }

        case Accelerator::BVH: {
            bvhPlanes_ = ::MobileRT::BVH {::std::move(scene_.planes_)};
            bvhSpheres_ = ::MobileRT::BVH {::std::move(scene_.spheres_)};
            bvhTriangles_ = ::MobileRT::BVH {::std::move(scene_.triangles_)};
            break;
        }
    }
}

bool Shader::rayTrace(::glm::vec3 *rgb, const Ray &ray) noexcept {
    Intersection intersection {RayLengthMax};
    bool intersected {false};
    switch (accelerator_) {
        case Accelerator::NAIVE: {
            intersected |= this->naivePlanes_.trace(&intersection, ray);
            intersected |= this->naiveSpheres_.trace(&intersection, ray);
            intersected |= this->naiveTriangles_.trace(&intersection, ray);
            intersected |= this->scene_.traceLights(&intersection, ray);
            break;
        }

        case Accelerator::REGULAR_GRID: {
            intersected |= this->regularGridPlanes_.trace(&intersection, ray);
            intersected |= this->regularGridSpheres_.trace(&intersection, ray);
            intersected |= this->regularGridTriangles_.trace(&intersection, ray);
            intersected |= this->scene_.traceLights(&intersection, ray);
            break;
        }

        case Accelerator::BVH: {
            intersected |= this->bvhPlanes_.trace(&intersection, ray);
            intersected |= this->bvhSpheres_.trace(&intersection, ray);
            intersected |= this->bvhTriangles_.trace(&intersection, ray);
            intersected |= this->scene_.traceLights(&intersection, ray);
            break;
        }
    }
    if (intersected) {
        if (intersection.materialId_ >= 0) {
            intersection.material_ = &scene_.materials_[static_cast<::std::size_t> (intersection.materialId_)];
        }
        const bool intersectedLight {shade(rgb, intersection, ray)};
        return intersectedLight;
    }
    return false;
}

bool Shader::shadowTrace(const Ray &ray, const float dist) noexcept {
    bool intersected {false};
    switch (accelerator_) {
        case Accelerator::NAIVE: {
            intersected |= this->naivePlanes_.shadowTrace(ray, dist);
            intersected |= this->naiveSpheres_.shadowTrace(ray, dist);
            intersected |= this->naiveTriangles_.shadowTrace(ray, dist);
            break;
        }

        case Accelerator::REGULAR_GRID: {
            intersected |= this->regularGridPlanes_.shadowTrace(ray, dist);
            intersected |= this->regularGridSpheres_.shadowTrace(ray, dist);
            intersected |= this->regularGridTriangles_.shadowTrace(ray, dist);
            break;
        }

        case Accelerator::BVH: {
            intersected |= this->bvhPlanes_.shadowTrace(ray, dist);
            intersected |= this->bvhSpheres_.shadowTrace(ray, dist);
            intersected |= this->bvhTriangles_.shadowTrace(ray, dist);
            break;
        }
    }
    return intersected;
}

void Shader::resetSampling() noexcept {
    this->scene_.resetSampling();
}

::glm::vec3 Shader::getCosineSampleHemisphere(const ::glm::vec3 &normal) const noexcept {
    thread_local static ::std::uniform_real_distribution<float> uniform_dist {0.0f, 1.0f};
    thread_local static ::std::random_device randomDevice {"/dev/urandom"};
    thread_local static ::std::mt19937 generator {randomDevice()};
    const float uniformRandom1 {uniform_dist(generator)};
    const float uniformRandom2 {uniform_dist(generator)};

    const float phi{
            ::glm::two_pi<float> () * uniformRandom1};// random angle around - azimuthal angle
    const float r2{uniformRandom2};// random distance from center
    const float cosTheta{::std::sqrt(
            r2)};// square root of distance from center - cos(theta) = cos(elevation angle)

    ::glm::vec3 u{::std::abs(normal[0]) > 0.1f ? ::glm::vec3 {0.0f, 1.0f, 0.0f} :
                  ::glm::vec3 {1.0f, 0.0f, 0.0f}};
    u = ::glm::normalize(::glm::cross(u, normal));// second axis
    const ::glm::vec3 &v{::glm::cross(normal, u)};// final axis

    ::glm::vec3 direction{u * (::std::cos(phi) * cosTheta) +
                          v * (::std::sin(phi) * cosTheta) +
                          normal * ::std::sqrt(1.0f - r2)};
    direction = ::glm::normalize(direction);

    /*float phi2 {::std::acos(::std::sqrt(1.0f - uniformRandom1))};
    float theta2 = ::glm::two_pi<float> () * r2;
    ::glm::vec3 dir {sin(phi2) * cos(theta2),
                    cos(phi2),
                     sin(phi2) * sin(theta2)};*/

    return direction;
}

::MobileRT::Light& Shader::getLight () {
    thread_local static ::std::uniform_real_distribution<float> uniform_dist {0.0f, 1.0f};
    thread_local static ::std::random_device randomDevice {"/dev/urandom"};
    thread_local static ::std::mt19937 generator {randomDevice()};
    const float randomNumber {uniform_dist(generator)};

    const ::std::uint32_t sizeLights {static_cast<::std::uint32_t> (scene_.lights_.size())};
    const ::std::uint32_t chosenLight {
        static_cast<::std::uint32_t> (::std::floor(randomNumber * sizeLights * 0.99999f))};
    Light &light {*scene_.lights_[chosenLight]};
    return light;
}
