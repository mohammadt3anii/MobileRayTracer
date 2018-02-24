//
// Created by Tiago on 16-10-2016.
//

#include "Plane.hpp"

using ::MobileRT::AABB;
using ::MobileRT::Plane;
using ::MobileRT::Intersection;

Plane::Plane(const ::glm::vec3 point, const ::glm::vec3 normal) noexcept :
        normal_{::glm::normalize(normal)},
        point_{point} {
}

Intersection Plane::intersect(Intersection intersection, const Ray ray) const noexcept {
    if (ray.primitive_ == this) {
        return intersection;
    }

    // is ray parallel or contained in the Plane ??
    // planes have two sides!!!
    //const float normalized_projection {this->normal_.dotProduct(ray.direction_)};
    const float normalized_projection {::glm::dot(normal_, ray.direction_)};
    if (::std::fabs(normalized_projection) < Epsilon) {
        return intersection;
    }

    //https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
    const ::glm::vec3 vecToOrigin {point_ - ray.origin_};
    const float distanceToIntersection {
            ::glm::dot(normal_, vecToOrigin) / normalized_projection};

    // is it in front of the eye?
    // is it farther than the ray length ??
    if (distanceToIntersection < Epsilon || distanceToIntersection >= intersection.length_) {
        return intersection;
    }

    // if so, then we have an intersection
    return Intersection {ray.origin_, ray.direction_, distanceToIntersection, normal_, this};
}

void Plane::moveTo(const float /*x*/, const float /*y*/) noexcept {
}

float Plane::getZ() const noexcept {
    return 0.0f;
}

::glm::vec3 Plane::getRightVector() const noexcept {
    ::glm::vec3 right {};
    if (this->normal_.x >= 1) {
        right = ::glm::vec3 {0, 1, 1};
    } else if (this->normal_.y >= 1) {
        right = ::glm::vec3 {1, 0, 1};
    } else if (this->normal_.z >= 1) {
        right = ::glm::vec3 {1, 1, 0};
    } else if (this->normal_.x <= -1) {
        right = ::glm::vec3 {0, 1, 1};
    } else if (this->normal_.y <= -1) {
        right = ::glm::vec3 {1, 0, 1};
    } else if (this->normal_.z <= -1) {
        right = ::glm::vec3 {1, 1, 0};
    }
    right = ::glm::normalize(right);
    return right;
}

::glm::vec3 Plane::getPositionMin() const noexcept {
    const ::glm::vec3 rightDir {getRightVector()};
    return this->point_ +  rightDir * -100.0f;
}

::glm::vec3 Plane::getPositionMax() const noexcept {
    const ::glm::vec3 rightDir {getRightVector()};
    return this->point_ + rightDir * 100.0f;
}

AABB Plane::getAABB() const noexcept {
    const ::glm::vec3 min {getPositionMin() - Epsilon};
    const ::glm::vec3 max {getPositionMax()};
    return AABB {min, max};;
}

float Plane::distance(const ::glm::vec3 point) const noexcept {
    //Plane Equation
    //a(x-x0)+b(y-y0)+c(z-z0) = 0
    //abc = normal
    //x0,y0,z0 = point
    //D = |ax0 + by0 + cz0 + d| / sqrt(a² + b² + c²)
    const float d {
            normal_.x * -point_.x + normal_.y * -point_.y +
            normal_.z * -point_.z};
    const float numerator {
            normal_.x * point.x + normal_.y * point.y + normal_.z * point.z + d};
    const float denumerator {
            ::std::sqrt(
                    normal_.x * normal_.x + normal_.y * normal_.y +
                    normal_.z * normal_.z)};
    return numerator / denumerator;
}

bool Plane::intersect(const AABB box) const noexcept {
    ::glm::vec3 positiveVertex {box.pointMax_};
    ::glm::vec3 negativeVertex {box.pointMin_};

    /*if (this->normal_.x_() >= 0.0f) {
      positiveVertex.x_() = box.pointMax_.x_;
      negativeVertex.x_() = box.pointMin_.x_;
    } else if (this->normal_.y_ >= 0.0f) {
      positiveVertex.y_ = box.pointMax_.y_;
      negativeVertex.y_ = box.pointMin_.y_;
    } else if (this->normal_.z_ >= 0.0f) {
      positiveVertex.z_ = box.pointMax_.z_;
      negativeVertex.z_ = box.pointMin_.z_;
    } else if (this->normal_.x_() < 0.0f) {
      positiveVertex.x_() = box.pointMin_.x_;
      negativeVertex.x_() = box.pointMax_.x_;
    } else if (this->normal_.y_ < 0.0f) {
      positiveVertex.y_ = box.pointMin_.y_;
      negativeVertex.y_ = box.pointMax_.y_;
    } else if (this->normal_.z_ < 0.0f) {
      positiveVertex.z_ = box.pointMin_.z_;
      negativeVertex.z_ = box.pointMax_.z_;
    }*/

    const float distanceP {distance(positiveVertex)};
    const float distanceN {distance(negativeVertex)};
    const bool res {(distanceP <= 0 && distanceN >= 0) || (distanceP >= 0 && distanceN <= 0)};

    return res;
}
