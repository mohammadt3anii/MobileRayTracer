//
// Created by Tiago on 15-11-2016.
//

#include "Triangle.h"

using namespace MobileRT;

Triangle::Triangle (const Point3D& pointA, const Point3D& pointB, const Point3D& pointC) :
    pointA_(pointA),
    pointB_(pointB),
    pointC_(pointC),
    AB_(pointB - pointA),
    AC_(pointC - pointA),
    BC_(pointC - pointB),
    normal_(AC_.crossProduct(AB_))
{
    this->normal_.normalize();
}

bool Triangle::intersect(Intersection& intersection, const Ray& ray, const Material& material) const
{
    const float a (ray.direction_.dotProduct(normal_));

    if (a == 0) return false;//raio paralelo ao triangulo

    const float distanceTriangle (normal_.dotProduct(pointA_));
    const float b = this->normal_.dotProduct(ray.origin_ + ((this->normal_*distanceTriangle).symmetric()));
    const float distanceIntersectCamera (-b / a);//distancia da interseçao à camera

    const float Qx ((ray.direction_*distanceIntersectCamera).x_ + ray.origin_.x_);
    const float Qy ((ray.direction_*distanceIntersectCamera).y_ + ray.origin_.y_);
    const float Qz ((ray.direction_*distanceIntersectCamera).z_ + ray.origin_.z_);

    const Point3D Q (Qx, Qy, Qz);

    //CAxQA * n >= 0
    Vector3D AQ (Q - pointA_);
    const float test1 (AC_.crossProduct(AQ).dotProduct(normal_));
    //BCxQC * n >= 0
    Vector3D CQ (Q - pointC_);
    const float test2 (BC_.crossProduct(CQ).dotProduct(normal_));
    //ABxQB * n >= 0
    Vector3D BQ (Q - pointB_);
    const float test3 (AB_.crossProduct(BQ).dotProduct(normal_));

    if (test1 < 0 || test2 < 0 || test3 < 0) return false;//está fora do trianglo

    intersection.length_ = distanceIntersectCamera;

    intersection.recycle(
        ray.origin_ + (ray.direction_ * distanceIntersectCamera),
        this->normal_,
        distanceIntersectCamera,
        material);

    return true;
}