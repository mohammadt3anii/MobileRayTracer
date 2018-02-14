//
// Created by Tiago on 16-10-2016.
//

#include "Sphere.hpp"

using ::MobileRT::AABB;
using ::MobileRT::Sphere;
using ::MobileRT::Point3D;

Sphere::Sphere(const Point3D center, const float radius) noexcept :
        center_{center},
        sq_radius_{radius * radius} {
}

bool Sphere::intersect(Intersection *const intersection, const Ray ray) const noexcept {
    //stackoverflow.com/questions/1986378/how-to-set-up-quadratic-equation-for-a-ray-sphere-intersection
    const Vector3D centerToOrigin{ray.origin_, this->center_};

    //A = 1.0 - normalized vectors
    const float B{2.0f * centerToOrigin.dotProduct(ray.direction_)};
    const float C{centerToOrigin.squareMagnitude() - this->sq_radius_};
    const float discriminant{B * B - 4.0f * C};
    //don't intersect (ignores tangent point of the sphere)
    if (discriminant <= 0.0f) { return false; }

    //ray intersects the sphere in 2 points
    const float rootDiscriminant{::std::sqrt(discriminant)};
    const float distanceToIntersection1{(-B + rootDiscriminant) * 0.5f};
    const float distanceToIntersection2{(-B - rootDiscriminant) * 0.5f};
    //distance between intersection and camera = smaller root = closer intersection
    const float distanceToIntersection{
            distanceToIntersection1 < distanceToIntersection2 ?
            distanceToIntersection1 : distanceToIntersection2};

    if (distanceToIntersection < Epsilon || distanceToIntersection > intersection->length_) {
        return false;
    }

    // if so, then we have an intersection
    intersection->reset(ray.origin_, ray.direction_, distanceToIntersection, this->center_);
    return true;
}

void Sphere::moveTo(const float x, const float y) noexcept {
    this->center_.position_.at(0) = x;
    this->center_.position_.at(1) = y;
}

float Sphere::getZ() const noexcept {
    return this->center_.z_();
}

Point3D Sphere::getPositionMin() const noexcept {
    const float radius{::std::sqrt(this->sq_radius_)};
    const float x{this->center_.x_() - radius};
    const float y{this->center_.y_() - radius};
    const float z{this->center_.z_() - radius};

    return Point3D {x, y, z};
}

Point3D Sphere::getPositionMax() const noexcept {
    const float radius{::std::sqrt(this->sq_radius_)};
    const float x{this->center_.x_() + radius};
    const float y{this->center_.y_() + radius};
    const float z{this->center_.z_() + radius};

    return Point3D {x, y, z};
}

AABB Sphere::getAABB() const noexcept {
    return AABB {getPositionMin(), getPositionMax()};
}

bool Sphere::intersect(const AABB box) const noexcept {
    float dmin{0};
    const Point3D v1{box.pointMin_};
    const Point3D v2{box.pointMax_};
    if (center_.x_() < v1.x_()) {
        dmin = dmin + (center_.x_() - v1.x_()) * (center_.x_() - v1.x_());
    } else if (center_.x_() > v2.x_()) {
        dmin = dmin + (center_.x_() - v2.x_()) * (center_.x_() - v2.x_());
    }
    if (center_.y_() < v1.y_()) {
        dmin = dmin + (center_.y_() - v1.y_()) * (center_.y_() - v1.y_());
    } else if (center_.y_() > v2.y_()) {
        dmin = dmin + (center_.y_() - v2.y_()) * (center_.y_() - v2.y_());
    }
    if (center_.z_() < v1.z_()) {
        dmin = dmin + (center_.z_() - v1.z_()) * (center_.z_() - v1.z_());
    } else if (center_.z_() > v2.z_()) {
        dmin = dmin + (center_.z_() - v2.z_()) * (center_.z_() - v2.z_());
    }
    const bool res{(dmin <= sq_radius_)};
    return res;
}
