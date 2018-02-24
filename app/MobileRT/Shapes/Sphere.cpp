//
// Created by Tiago on 16-10-2016.
//

#include "Sphere.hpp"

using ::MobileRT::AABB;
using ::MobileRT::Sphere;
using ::MobileRT::Intersection;

Sphere::Sphere(const ::glm::vec3 center, const float radius) noexcept :
        center_{center},
        sq_radius_{radius * radius} {
}

Intersection Sphere::intersect(Intersection intersection, const Ray ray) const noexcept {
    //stackoverflow.com/questions/1986378/how-to-set-up-quadratic-equation-for-a-ray-sphere-intersection
    const ::glm::vec3 centerToOrigin {ray.origin_ - center_};

    //A = 1.0 - normalized vectors
    const float B {2.0f * ::glm::dot(centerToOrigin, ray.direction_)};
    const float squareMagnitude {::glm::length(centerToOrigin) * ::glm::length(centerToOrigin)};
    const float C {squareMagnitude - this->sq_radius_};
    const float discriminant {B * B - 4.0f * C};
    //don't intersect (ignores tangent point of the sphere)
    if (discriminant <= 0.0f) { return intersection; }

    //ray intersects the sphere in 2 points
    const float rootDiscriminant {::std::sqrt(discriminant)};
    const float distanceToIntersection1 {(-B + rootDiscriminant) * 0.5f};
    const float distanceToIntersection2 {(-B - rootDiscriminant) * 0.5f};
    //distance between intersection and camera = smaller root = closer intersection
    const float distanceToIntersection {::std::min(distanceToIntersection1, distanceToIntersection2)};

    if (distanceToIntersection < Epsilon || distanceToIntersection >= intersection.length_) {
        return intersection;
    }

    // if so, then we have an intersection
    const ::glm::vec3 intersectionPoint {ray.origin_ + ray.direction_ * distanceToIntersection};
    return Intersection {intersectionPoint, distanceToIntersection, center_};
}

void Sphere::moveTo(const float x, const float y) noexcept {
    this->center_.x = x;
    this->center_.y = y;
}

float Sphere::getZ() const noexcept {
    return this->center_.z;
}

AABB Sphere::getAABB() const noexcept {
    const float radius {::std::sqrt(this->sq_radius_)};
    const ::glm::vec3 min {this->center_ - radius};
    const ::glm::vec3 max {this->center_ + radius};
    return AABB {min, max};
}

bool Sphere::intersect(const AABB box) const noexcept {
    float dmin {0};
    const ::glm::vec3 v1 {box.pointMin_};
    const ::glm::vec3 v2 {box.pointMax_};
    if (center_.x < v1.x) {
        dmin = dmin + (center_.x - v1.x) * (center_.x - v1.x);
    } else if (center_.x > v2.x) {
        dmin = dmin + (center_.x - v2.x) * (center_.x - v2.x);
    }
    if (center_.y < v1.y) {
        dmin = dmin + (center_.y - v1.y) * (center_.y - v1.y);
    } else if (center_.y > v2.y) {
        dmin = dmin + (center_.y - v2.y) * (center_.y - v2.y);
    }
    if (center_.z < v1.z) {
        dmin = dmin + (center_.z - v1.z) * (center_.z - v1.z);
    } else if (center_.z > v2.z) {
        dmin = dmin + (center_.z - v2.z) * (center_.z - v2.z);
    }
    const bool res {(dmin <= sq_radius_)};
    return res;
}
