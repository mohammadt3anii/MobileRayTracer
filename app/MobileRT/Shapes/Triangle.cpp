//
// Created by Tiago on 15-11-2016.
// Uses: Moller and Trumbore
//

#include "Triangle.hpp"

using ::MobileRT::AABB;
using ::MobileRT::Triangle;
using ::MobileRT::Intersection;

Triangle::Triangle(const ::glm::vec3 pointA, const ::glm::vec3 pointB, const ::glm::vec3 pointC,
                   ::glm::vec3 normal) noexcept :
        AC_{pointC - pointA},
        AB_{pointB - pointA},
        normal_{
            ::glm::all(::glm::lessThan(normal, ::glm::vec3(::MobileRT::Epsilon)))? ::glm::normalize(::glm::cross(AB_, AC_)) : ::glm::normalize(normal)},
        pointA_{pointA}
{
}

Intersection Triangle::intersect(Intersection intersection, const Ray ray) const noexcept {
    if (ray.primitive_ == this) {
        return intersection;
    }

    const ::glm::vec3 perpendicularVector {::glm::cross(ray.direction_, AC_)};
    const float normalizedProjection {::glm::dot(AB_, perpendicularVector)};
    if (::std::abs(normalizedProjection) < Epsilon) {
        return intersection;
    }

    const float normalizedProjectionInv {1.0f / normalizedProjection};
    const ::glm::vec3 vectorToCamera {ray.origin_ - pointA_};
    const float u {normalizedProjectionInv * ::glm::dot(vectorToCamera, perpendicularVector)};
    if (u < 0.0f || u > 1.0f) {
        return intersection;
    }

    const ::glm::vec3 upPerpendicularVector {::glm::cross(vectorToCamera, AB_)};
    const float v {normalizedProjectionInv * ::glm::dot (ray.direction_, upPerpendicularVector)};
    if (v < 0.0f || (u + v) > 1.0f) {
        return intersection;
    }

    // at this stage we can compute t to find out where
    // the intersection point is on the line
    const float distanceToIntersection {normalizedProjectionInv * ::glm::dot(AC_, upPerpendicularVector)};

    if (distanceToIntersection < Epsilon || distanceToIntersection >= intersection.length_) {
        return intersection;
    }
    return Intersection {ray.origin_, ray.direction_, distanceToIntersection, normal_, this};
}

void Triangle::moveTo(const float /*x*/, const float /*y*/) noexcept {
}

float Triangle::getZ() const noexcept {
    return 0.0f;
}

::glm::vec3 Triangle::getPositionMin() const noexcept {
    const ::glm::vec3 pointB {pointA_ + AB_};
    const ::glm::vec3 pointC {pointA_ + AC_};
    const ::glm::vec3 res {::glm::min(pointA_, ::glm::min(pointB, pointC))};
    return res;
}

::glm::vec3 Triangle::getPositionMax() const noexcept {
    const ::glm::vec3 pointB {pointA_ + AB_};
    const ::glm::vec3 pointC {pointA_ + AC_};
    const ::glm::vec3 res {::glm::max(pointA_, ::glm::max(pointB, pointC))};
    return res;
}

//TODO (Puscas): Fix this method (it may be wrong)
AABB Triangle::getAABB() const noexcept {
    return AABB {getPositionMin() - Epsilon, getPositionMax()};
}

bool Triangle::intersect(const AABB box) const noexcept {
    ::std::function<bool(const ::glm::vec3, const ::glm::vec3)> intersectRayAABB{
            [=](const ::glm::vec3 orig, const ::glm::vec3 vec) -> bool {
                ::glm::vec3 T_1{};
                ::glm::vec3 T_2{}; // vectors to hold the T-values for every direction
                float t_near{::std::numeric_limits<float>::min()};
                float t_far{::std::numeric_limits<float>::max()};
                if (vec.x == 0) { // ray parallel to planes in this direction
                    if ((orig.x < box.pointMin_.x) ||
                        ((orig.x + vec.x) > box.pointMax_.x)) {
                        return false; // parallel AND outside box : no intersection possible
                    }
                } else { // ray not parallel to planes in this direction
                    T_1.x = ((box.pointMin_.x - orig.x) / vec.x);
                    T_2.x = ((box.pointMax_.x - orig.x) / vec.x);
                    if (T_1.x >
                        T_2.x) { // we want T_1 to hold values for intersection with near plane
                        ::std::swap(T_1, T_2);
                    }
                    t_near = ::std::max(T_1.x, t_near);
                    t_far = ::std::min(T_2.x, t_far);
                    if ((t_near > t_far) || (t_far < 0)) {
                        return false;
                    }
                }
                if (vec.y == 0) { // ray parallel to planes in this direction
                    if ((orig.y < box.pointMin_.y) ||
                        ((orig.y + vec.y) > box.pointMax_.y)) {
                        return false; // parallel AND outside box : no intersection possible
                    }
                } else { // ray not parallel to planes in this direction
                    T_1.y = ((box.pointMin_.y - orig.y) / vec.y);
                    T_2.y = ((box.pointMax_.y - orig.y) / vec.y);
                    if (T_1.y >
                        T_2.y) { // we want T_1 to hold values for intersection with near plane
                        ::std::swap(T_1, T_2);
                    }
                    t_near = ::std::max(T_1.y, t_near);
                    t_far = ::std::min(T_2.y, t_far);
                    if ((t_near > t_far) || (t_far < 0)) {
                        return false;
                    }
                }
                if (vec.z == 0) { // ray parallel to planes in this direction
                    if ((orig.z < box.pointMin_.z) ||
                        ((orig.z + vec.z) > box.pointMax_.z)) {
                        return false; // parallel AND outside box : no intersection possible
                    }
                } else { // ray not parallel to planes in this direction
                    T_1.z = ((box.pointMin_.z - orig.z) / vec.z);
                    T_2.z = ((box.pointMax_.z - orig.z) / vec.z);
                    if (T_1.z >
                        T_2.z) { // we want T_1 to hold values for intersection with near plane
                        ::std::swap(T_1, T_2);
                    }
                    t_near = ::std::max(T_1.z, t_near);
                    t_far = ::std::min(T_2.z, t_far);
                    if ((t_near > t_far) || (t_far < 0)) {
                        return false;
                    }
                }
                return true; // if we made it here, there was an intersection - YAY
            }};

    ::std::function<bool(const ::glm::vec3)> isOverTriangle {
            [=](const ::glm::vec3 vec) -> bool {
                const ::glm::vec3 perpendicularVector {::glm::cross(vec, AC_)};
                const float normalizedProjection {::glm::dot(AB_, perpendicularVector)};
                return ::std::abs(normalizedProjection) < Epsilon;
            }
    };

    const ::glm::vec3 &min(box.pointMin_);
    const ::glm::vec3 &max(box.pointMax_);
    Intersection intersection{};
    const ::glm::vec3 vec{max - min};
    const Ray ray{vec, min, 1};
    const bool intersectedAB{intersectRayAABB(this->pointA_, this->AB_)};
    const bool intersectedAC{intersectRayAABB(this->pointA_, this->AC_)};
    const ::glm::vec3 pointB {pointA_ + AB_};
    const ::glm::vec3 pointC {pointA_ + AC_};
    const bool intersectedBC{intersectRayAABB(pointB, pointC - pointB)};
    const float dist {intersection.length_};
    intersection = intersect(intersection, ray);
    const bool intersectedRay {intersection.length_ < dist};
    const bool insideTriangle{isOverTriangle(vec)};

    return intersectedAB || intersectedAC || intersectedBC || intersectedRay || insideTriangle;
}
