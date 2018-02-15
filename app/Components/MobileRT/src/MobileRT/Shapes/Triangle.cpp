//
// Created by Tiago on 15-11-2016.
// Uses: Moller and Trumbore
//

#include "Triangle.hpp"

using ::MobileRT::AABB;
using ::MobileRT::Triangle;
using ::MobileRT::Point3D;
using ::MobileRT::Vector3D;
using ::MobileRT::Intersection;

Triangle::Triangle(const Point3D pointA, const Point3D pointB, const Point3D pointC,
                   Vector3D normal) noexcept :
        AC_{pointC - pointA},
        AB_{pointB - pointA},
        BC_{pointC - pointB},
        normal_{
                normal.isNull() ? AB_.crossProduct(AC_).returnNormalized()
                                : normal.returnNormalized()},
        pointA_{pointA},
        pointB_{pointB},
        pointC_{pointC} {
}

Intersection Triangle::intersect(Intersection intersection, const Ray ray) const noexcept {
    if (ray.primitive_ == this) {
        return intersection;
    }
    const Vector3D perpendicularVector{ray.direction_, this->AC_};//cross product
    const float normalizedProjection{this->AB_.dotProduct(perpendicularVector)};
    if (::std::fabs(normalizedProjection) < Epsilon) {
        return intersection;
    }

    const float normalizedProjectionInv{1.0f / normalizedProjection};
    const Vector3D vectorToCamera{ray.origin_, this->pointA_};
    const float u{normalizedProjectionInv * vectorToCamera.dotProduct(perpendicularVector)};
    if (u < 0.0f || u > 1.0f) {
        return intersection;
    }

    const Vector3D upPerpendicularVector{vectorToCamera, this->AB_};//cross product
    const float v{normalizedProjectionInv * ray.direction_.dotProduct(upPerpendicularVector)};
    if (v < 0.0f || (u + v) > 1.0f) {
        return intersection;
    }

    // at this stage we can compute t to find out where
    // the intersection point is on the line
    const float distanceToIntersection{
            normalizedProjectionInv * this->AC_.dotProduct(upPerpendicularVector)};

    if (distanceToIntersection < Epsilon || distanceToIntersection >= intersection.length_) {
        return intersection;
    }
    return Intersection {ray.origin_, ray.direction_, distanceToIntersection, this->normal_, this};
}

void Triangle::moveTo(const float /*x*/, const float /*y*/) noexcept {
}

float Triangle::getZ() const noexcept {
    return 0.0f;
}

Point3D Triangle::getPositionMin() const noexcept {
    const float x{::std::min(pointA_.x_(), ::std::min(pointB_.x_(), pointC_.x_()))};
    const float y{::std::min(pointA_.y_(), ::std::min(pointB_.y_(), pointC_.y_()))};
    const float z{::std::min(pointA_.z_(), ::std::min(pointB_.z_(), pointC_.z_()))};
    return Point3D(x, y, z);
}

Point3D Triangle::getPositionMax() const noexcept {
    const float x{::std::max(pointA_.x_(), ::std::max(pointB_.x_(), pointC_.x_()))};
    const float y{::std::max(pointA_.y_(), ::std::max(pointB_.y_(), pointC_.y_()))};
    const float z{::std::max(pointA_.z_(), ::std::max(pointB_.z_(), pointC_.z_()))};
    return Point3D(x, y, z);
}

//TODO (Puscas): Fix this method (it may be wrong)
AABB Triangle::getAABB() const noexcept {
    return AABB {getPositionMin() - Epsilon, getPositionMax()};
}

bool Triangle::intersect(const AABB box) const noexcept {
    ::std::function<bool(const Point3D, const Vector3D)> intersectRayAABB{
            [&](const Point3D orig, const Vector3D vec) -> bool {
                Vector3D T_1{};
                Vector3D T_2{}; // vectors to hold the T-values for every direction
                float t_near{std::numeric_limits<float>::min()};
                float t_far{std::numeric_limits<float>::max()};
                if (vec.x_() == 0) { // ray parallel to planes in this direction
                    if ((orig.x_() < box.pointMin_.x_()) ||
                        ((orig.x_() + vec.x_()) > box.pointMax_.x_())) {
                        return false; // parallel AND outside box : no intersection possible
                    }
                } else { // ray not parallel to planes in this direction
                    T_1.setX((box.pointMin_.x_() - orig.x_()) / vec.x_());
                    T_2.setX((box.pointMax_.x_() - orig.x_()) / vec.x_());
                    if (T_1.x_() >
                        T_2.x_()) { // we want T_1 to hold values for intersection with near plane
                        ::std::swap(T_1, T_2);
                    }
                    t_near = ::std::max(T_1.x_(), t_near);
                    t_far = ::std::min(T_2.x_(), t_far);
                    if ((t_near > t_far) || (t_far < 0)) {
                        return false;
                    }
                }
                if (vec.y_() == 0) { // ray parallel to planes in this direction
                    if ((orig.y_() < box.pointMin_.y_()) ||
                        ((orig.y_() + vec.y_()) > box.pointMax_.y_())) {
                        return false; // parallel AND outside box : no intersection possible
                    }
                } else { // ray not parallel to planes in this direction
                    T_1.setY((box.pointMin_.y_() - orig.y_()) / vec.y_());
                    T_2.setY((box.pointMax_.y_() - orig.y_()) / vec.y_());
                    if (T_1.y_() >
                        T_2.y_()) { // we want T_1 to hold values for intersection with near plane
                        ::std::swap(T_1, T_2);
                    }
                    t_near = ::std::max(T_1.y_(), t_near);
                    t_far = ::std::min(T_2.y_(), t_far);
                    if ((t_near > t_far) || (t_far < 0)) {
                        return false;
                    }
                }
                if (vec.z_() == 0) { // ray parallel to planes in this direction
                    if ((orig.z_() < box.pointMin_.z_()) ||
                        ((orig.z_() + vec.z_()) > box.pointMax_.z_())) {
                        return false; // parallel AND outside box : no intersection possible
                    }
                } else { // ray not parallel to planes in this direction
                    T_1.setZ((box.pointMin_.z_() - orig.z_()) / vec.z_());
                    T_2.setZ((box.pointMax_.z_() - orig.z_()) / vec.z_());
                    if (T_1.z_() >
                        T_2.z_()) { // we want T_1 to hold values for intersection with near plane
                        ::std::swap(T_1, T_2);
                    }
                    t_near = ::std::max(T_1.z_(), t_near);
                    t_far = ::std::min(T_2.z_(), t_far);
                    if ((t_near > t_far) || (t_far < 0)) {
                        return false;
                    }
                }
                return true; // if we made it here, there was an intersection - YAY
            }};

    ::std::function<bool(const Vector3D)> isOverTriangle{
            [&](const Vector3D vec) -> bool {
                const Vector3D perpendicularVector{vec, this->AC_};//cross product
                const float normalizedProjection{this->AB_.dotProduct(perpendicularVector)};
                return ::std::fabs(normalizedProjection) < Epsilon;
            }
    };

    const Point3D &min(box.pointMin_);
    const Point3D &max(box.pointMax_);
    Intersection intersection{};
    const Vector3D vec{max, min};
    const Ray ray{vec, min, 1};
    bool intersectedAB{intersectRayAABB(this->pointA_, this->AB_)};
    bool intersectedAC{intersectRayAABB(this->pointA_, this->AC_)};
    bool intersectedBC{intersectRayAABB(this->pointB_, this->BC_)};
    intersection = intersect(intersection, ray);
    bool insideTriangle{isOverTriangle(vec)};

    return intersectedAB || intersectedAC || intersectedBC || /*intersectedRay ||*/ insideTriangle;
}
