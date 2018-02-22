//
// Created by Tiago on 10-Jul-17.
//

#include "Rectangle.hpp"

using ::MobileRT::AABB;
using ::MobileRT::Rectangle;
using ::MobileRT::Point3D;
using ::MobileRT::Intersection;

Rectangle::Rectangle(const Point3D pointA,
                     const Point3D pointB,
                     const Point3D pointC,
                     const Point3D pointD) noexcept :
        pointA_{pointA},
        pointB_{pointB},
        pointC_{pointC},
        pointD_{pointD},
        AC_{pointC_ - pointA_},
        AB_{pointB_ - pointA_},
        BD_{pointD_ - pointB_},
        CD_{pointD_ - pointC_},
        normal_{AB_.crossProduct(AC_)} {
}

Intersection Rectangle::intersect(Intersection intersection, const Ray ray) const noexcept {
    const Vector3D perpendicularVector{ray.direction_, this->AC_};
    const float normalizedProjection{this->AB_.dotProduct(perpendicularVector)};
    if (::std::fabs(normalizedProjection) < Epsilon) {
        return intersection;
    }

    const float normalizedProjectionInv{1.0f / normalizedProjection};
    const Vector3D vertexToCamera{ray.origin_, this->pointA_};
    const float u{normalizedProjectionInv * vertexToCamera.dotProduct(perpendicularVector)};
    if (u < 0.0f || u > 1.0f) {
        return intersection;
    }

    const Vector3D upPerpendicularVector{vertexToCamera, this->AB_};//cross product
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
    return Intersection {ray.origin_, ray.direction_, distanceToIntersection, this->normal_};
}

void Rectangle::moveTo(const float /*x*/, const float /*y*/) noexcept {
}

float Rectangle::getZ() const noexcept {
    return 0.0f;
}

Point3D Rectangle::getPositionMin() const noexcept {
    const float x{::std::min(pointA_.x_(), ::std::min(pointB_.x_(),
        ::std::min(pointC_.x_(), pointD_.x_())))};
    const float y{::std::min(pointA_.y_(), ::std::min(pointB_.y_(),
        ::std::min(pointC_.y_(), pointD_.y_())))};
    const float z{::std::min(pointA_.z_(), ::std::min(pointB_.z_(),
        ::std::min(pointC_.z_(), pointD_.z_())))};

    return Point3D(x, y, z);
}

Point3D Rectangle::getPositionMax() const noexcept {
    const float x{::std::max(pointA_.x_(), ::std::max(pointB_.x_(),
        ::std::max(pointC_.x_(), pointD_.x_())))};
    const float y{::std::max(pointA_.y_(), ::std::max(pointB_.y_(),
        ::std::max(pointC_.y_(), pointD_.y_())))};
    const float z{::std::max(pointA_.z_(), ::std::max(pointB_.z_(),
        ::std::max(pointC_.z_(), pointD_.z_())))};

    return Point3D(x, y, z);
}

AABB Rectangle::getAABB() const noexcept {
    Point3D min{getPositionMin()};
    Point3D max{getPositionMax()};
    return AABB(min, max);
}

bool Rectangle::intersect(const AABB box) const noexcept {
    ::std::function<bool(Point3D orig, Vector3D vec)> intersectRayAABB{
            [=](Point3D orig, Vector3D vec) -> bool {
                Vector3D T_1{};
                Vector3D T_2{}; // vectors to hold the T-values for every direction
                float t_near{std::numeric_limits<float>::min()};
                float t_far{std::numeric_limits<float>::max()};
                if (vec.x_() == 0) { // ray parallel to planes in this direction
                    if ((orig.x_() < box.pointMin_.x_()) || (vec.x_() > box.pointMax_.x_())) {
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
                    if ((orig.y_() < box.pointMin_.y_()) || (vec.y_() > box.pointMax_.y_())) {
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
                    if ((orig.z_() < box.pointMin_.z_()) || (vec.z_() > box.pointMax_.z_())) {
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

    return intersectRayAABB(this->pointA_, this->AB_) ||
           intersectRayAABB(this->pointA_, this->AC_) ||
           intersectRayAABB(this->pointB_, this->BD_) ||
           intersectRayAABB(this->pointC_, this->CD_);
}