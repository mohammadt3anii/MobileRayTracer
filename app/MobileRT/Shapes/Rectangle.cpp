//
// Created by Tiago on 10-Jul-17.
//

#include "Rectangle.hpp"

using ::MobileRT::AABB;
using ::MobileRT::Rectangle;
using ::MobileRT::Intersection;

Rectangle::Rectangle(const ::glm::vec3 pointA,
                     const ::glm::vec3 pointB,
                     const ::glm::vec3 pointC,
                     const ::glm::vec3 pointD) noexcept :
        pointA_{pointA},
        pointB_{pointB},
        pointC_{pointC},
        pointD_{pointD},
        AC_{pointC_ - pointA_},
        AB_{pointB_ - pointA_},
        BD_{pointD_ - pointB_},
        CD_{pointD_ - pointC_},
        normal_{::glm::cross(AB_, AC_)} {
}

Intersection Rectangle::intersect(Intersection intersection, const Ray ray) const noexcept {
    if (ray.primitive_ == this) {
        return intersection;
    }

    const ::glm::vec3 perpendicularVector {::glm::cross(ray.direction_, this->AC_)};
    const float normalizedProjection {::glm::dot(AB_, perpendicularVector)};
    if (::std::fabs(normalizedProjection) < Epsilon) {
        return intersection;
    }

    const float normalizedProjectionInv {1.0f / normalizedProjection};
    const ::glm::vec3 vertexToCamera {ray.direction_ - this->pointA_};
    const float u {normalizedProjectionInv * ::glm::dot(vertexToCamera, perpendicularVector)};
    if (u < 0.0f || u > 1.0f) {
        return intersection;
    }

    const ::glm::vec3 upPerpendicularVector {::glm::cross(vertexToCamera, this->AB_)};
    const float v {normalizedProjectionInv * ::glm::dot(ray.direction_, upPerpendicularVector)};
    if (v < 0.0f || (u + v) > 1.0f) {
        return intersection;
    }

    // at this stage we can compute t to find out where
    // the intersection point is on the line
    const float distanceToIntersection{
            normalizedProjectionInv * ::glm::dot(this->AC_, upPerpendicularVector)};

    if (distanceToIntersection < Epsilon || distanceToIntersection >= intersection.length_) {
        return intersection;
    }
    return Intersection {ray.origin_, ray.direction_, distanceToIntersection, normal_};
}

void Rectangle::moveTo(const float /*x*/, const float /*y*/) noexcept {
}

float Rectangle::getZ() const noexcept {
    return 0.0f;
}

::glm::vec3 Rectangle::getPositionMin() const noexcept {
    const float x {::std::min(pointA_.x, ::std::min(pointB_.x,
        ::std::min(pointC_.x, pointD_.x)))};
    const float y {::std::min(pointA_.y, ::std::min(pointB_.y,
        ::std::min(pointC_.y, pointD_.y)))};
    const float z {::std::min(pointA_.z, ::std::min(pointB_.z,
        ::std::min(pointC_.z, pointD_.z)))};

    return ::glm::vec3(x, y, z);
}

::glm::vec3 Rectangle::getPositionMax() const noexcept {
    const float x{::std::max(pointA_.x, ::std::max(pointB_.x,
        ::std::max(pointC_.x, pointD_.x)))};
    const float y{::std::max(pointA_.y, ::std::max(pointB_.y,
        ::std::max(pointC_.y, pointD_.y)))};
    const float z{::std::max(pointA_.z, ::std::max(pointB_.z,
        ::std::max(pointC_.z, pointD_.z)))};

    return ::glm::vec3(x, y, z);
}

AABB Rectangle::getAABB() const noexcept {
    const ::glm::vec3 min{getPositionMin()};
    const ::glm::vec3 max{getPositionMax()};
    return AABB(min, max);
}

bool Rectangle::intersect(const AABB box) const noexcept {
    ::std::function<bool(const ::glm::vec3 orig, const ::glm::vec3 vec)> intersectRayAABB{
            [=](::glm::vec3 orig, ::glm::vec3 vec) -> bool {
                ::glm::vec3 T_1 {};
                ::glm::vec3 T_2 {}; // vectors to hold the T-values for every direction
                float t_near {std::numeric_limits<float>::min()};
                float t_far {std::numeric_limits<float>::max()};
                if (vec.x == 0) { // ray parallel to planes in this direction
                    if ((orig.x < box.pointMin_.x) || (vec.x > box.pointMax_.x)) {
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
                    if ((orig.y < box.pointMin_.y) || (vec.y > box.pointMax_.y)) {
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
                    if ((orig.z < box.pointMin_.z) || (vec.z > box.pointMax_.z)) {
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

    return intersectRayAABB(this->pointA_, this->AB_) ||
           intersectRayAABB(this->pointA_, this->AC_) ||
           intersectRayAABB(this->pointB_, this->BD_) ||
           intersectRayAABB(this->pointC_, this->CD_);
}
