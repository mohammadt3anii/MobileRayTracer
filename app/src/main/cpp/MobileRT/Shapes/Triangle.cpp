//
// Created by Tiago on 15-11-2016.
//

#include "Triangle.h"
#include "../Constants.h"

using namespace MobileRT;

Triangle::Triangle (const Point3D& pointA, const Point3D& pointB, const Point3D& pointC) :
    pointA_(pointA),
    pointB_(pointB),
    pointC_(pointC),
    AB_(pointB - pointA),
    BC_(pointC - pointB),
    CA_(pointA - pointC),
    normal_(AB_.crossProduct(pointC - pointA))
{
    this->normal_.normalize();
}

bool Triangle::intersect(Intersection& intersection, const Ray& ray, const Material& material) const
{
    const float a (this->normal_.dotProduct(ray.direction_));

    if (a < VECT_PROJ_MIN || a > -VECT_PROJ_MIN) return false;//raio paralelo ao triangulo

    //const float distanceTriangle (this->normal_.dotProduct(pointA_));
//const float b=this->normal_.dotProduct(ray.origin_+((this->normal_*distanceTriangle).negative()));
    const float D = normal_.dotProduct(pointA_);
    const float b = normal_.dotProduct(ray.origin_) + D;
    const float distanceIntersectCamera (-b / a);//distancia da interseçao à camera

    if (distanceIntersectCamera < 0) return false;//trianglo atrás da camera
/*
    const float Qx ((ray.direction_*distanceIntersectCamera).x_ + ray.origin_.x_);
    const float Qy ((ray.direction_*distanceIntersectCamera).y_ + ray.origin_.y_);
    const float Qz ((ray.direction_*distanceIntersectCamera).z_ + ray.origin_.z_);

    const Point3D Q (Qx, Qy, Qz);
*/
    Point3D intersectionPoint = (ray.origin_ + (ray.direction_ * distanceIntersectCamera));

    Vector3D perpendicularTriangle;
    
    perpendicularTriangle = AB_.crossProduct(intersectionPoint - pointA_);
    if (normal_.dotProduct(perpendicularTriangle) < 0) return false;

    perpendicularTriangle = BC_.crossProduct(intersectionPoint - pointB_);
    if (normal_.dotProduct(perpendicularTriangle) < 0) return false;

    perpendicularTriangle = CA_.crossProduct(intersectionPoint - pointC_);
    if (normal_.dotProduct(perpendicularTriangle) < 0) return false;

/*
    //CAxQA * n >= 0
    Vector3D AQ (Q - pointA_);
    const float test1 ((AC_.crossProduct(AQ)).dotProduct(this->normal_));
    //BCxQC * n >= 0
    Vector3D CQ (Q - pointC_);
    const float test2 ((BC_.crossProduct(CQ)).dotProduct(this->normal_));
    //ABxQB * n >= 0
    Vector3D BQ (Q - pointB_);
    const float test3 ((AB_.crossProduct(BQ)).dotProduct(this->normal_));

    if (test1 < 0 || test2 < 0 || test3 < 0) return false;//está fora do trianglo
*/
    intersection.recycle(
        ray.origin_ + (ray.direction_ * distanceIntersectCamera),
        this->normal_,
        distanceIntersectCamera,
        material);

    return true;
}