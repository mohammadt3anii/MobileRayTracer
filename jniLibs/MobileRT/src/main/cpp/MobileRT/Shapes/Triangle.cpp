//
// Created by Tiago on 15-11-2016.
// Uses: Moller and Trumbore
//

#include "Triangle.h"

using namespace MobileRT;

Triangle::Triangle(const Point3D &pointA, const Point3D &pointB, const Point3D &pointC) :
        pointA_(pointA),
        pointB_(pointB),
        pointC_(pointC),
        AB_(pointB - pointA),
        AC_(pointC - pointA),
        normal_(AB_.crossProduct(AC_))
{
    /*this->AB_.normalize();
    this->AC_.normalize();*/
    this->normal_.normalize();
    normal_.x_ = normal_.x_ == 0.0f? 0.0f : normal_.x_;
    normal_.y_ = normal_.y_ == 0.0f? 0.0f : normal_.y_;
    normal_.z_ = normal_.z_ == 0.0f? 0.0f : normal_.z_;
    LOG ("triangleAB=[%f, %f, %f]", double(AB_.x_),double(AB_.y_),double(AB_.z_));
    LOG ("triangleAC=[%f, %f, %f]", double(AC_.x_),double(AC_.y_),double(AC_.z_));
    LOG ("triangleNormal=[%f, %f, %f]", double(normal_.x_),double(normal_.y_),double(normal_.z_));
}

bool Triangle::intersect(Intersection &intersection, const Ray &ray,
                         const Material &material) const {
    const Vector3D perpendicularVector(ray.direction_, this->AC_);
    const float normalizedProjection(AB_.dotProduct(perpendicularVector));
    if (normalizedProjection * (1.0f + (normalizedProjection < 0.0f) * -2.0f) < VECT_PROJ_MIN)
        return false;

    const float normalizedProjectionInv(1.0f / normalizedProjection);

    const Vector3D vertexToCamera(ray.origin_, pointA_);

    const float u(normalizedProjectionInv * vertexToCamera.dotProduct(perpendicularVector));

    if (u < 0.0f || u > 1.0f)
        return false;

    const Vector3D upPerpendicularVector(vertexToCamera, this->AB_);//cross product
    const float v(normalizedProjectionInv * ray.direction_.dotProduct(upPerpendicularVector));

    if (v < 0.0f || (u + v) > 1.0f)
        return false;

    // at this stage we can compute t to find out where
    // the intersection point is on the line
    const float distanceToIntersection(
            normalizedProjectionInv * this->AC_.dotProduct(upPerpendicularVector));

    if (distanceToIntersection < RAY_LENGTH_MIN || distanceToIntersection > intersection.length_)
        return false;

    intersection.reset(
            ray.origin_, ray.direction_, distanceToIntersection,
            this->normal_,
            material,
            ray.origin_);

    return true;
}
