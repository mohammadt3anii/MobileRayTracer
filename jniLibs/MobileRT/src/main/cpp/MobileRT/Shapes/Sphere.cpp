//
// Created by Tiago on 16-10-2016.
//

#include "Sphere.h"

using namespace MobileRT;

Sphere::Sphere(const Point3D &center, const float radius) :
        sq_radius_(radius * radius),
        center_(center) {
}

bool Sphere::intersect(Intersection &intersection, const Ray &ray, const Material &material) const {
//stackoverflow.com/questions/1986378/how-to-set-up-quadratic-equation-for-a-ray-sphere-intersection
    const Vector3D centerToOrigin_(ray.origin_, this->center_);

    //A = 1.0 - normalized vectors
    const float B(2.0f * centerToOrigin_.dotProduct(ray.direction_));
    const float C(centerToOrigin_.squareLength() - this->sq_radius_);

    const float discriminant(B * B - 4.0f * C);
    //don't intersect (ignores tangent point of the sphere)
    if (discriminant <= 0.0f) return false;

    //ray intersects the sphere in 2 points
    const float rootDiscriminant(std::sqrt(discriminant));
    const float distanceToIntersection1((-B + rootDiscriminant) * 0.5f);
    const float distanceToIntersection2((-B - rootDiscriminant) * 0.5f);
    //distance between intersection and camera = smaller root = closer intersection
    const float distanceToIntersection((distanceToIntersection1 < distanceToIntersection2) *
                                       distanceToIntersection1 +
                                       (distanceToIntersection2 < distanceToIntersection1) *
                                       distanceToIntersection2);

    if (distanceToIntersection < RAY_LENGTH_MIN || distanceToIntersection > ray.maxDistance_)
        return false;

    const Point3D intersectionPoint(ray.origin_, ray.direction_, distanceToIntersection);
    Vector3D normal(intersectionPoint, this->center_);
    normal.normalize();
    //normal.resetAndNormalize(intersectionPoint, this->center_);

    // if so, then we have an intersection
    intersection.reset(
            intersectionPoint,
            normal,
            distanceToIntersection,
            material);

    return true;
}

void Sphere::moveTo(const float x, const float y) {
    this->center_.x_ = x;
    this->center_.y_ = y;
}

float Sphere::getZ(void) const {
    return this->center_.z_;
}
