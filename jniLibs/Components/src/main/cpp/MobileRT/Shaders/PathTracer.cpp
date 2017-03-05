//
// Created by puscas on 20-02-2017.
//

#include "PathTracer.h"

using namespace MobileRT;

PathTracer::PathTracer(const Scene &scene, Sampler &sampler) :
        Shader(scene),
        sampler_(sampler) {
}

void PathTracer::shade(RGB &rgb, Intersection &intersection, const Ray &ray) const {
    const RGB &kD(intersection.material_->Kd_);
    // the normal always points to outside objects (e.g., spheres)
    // if the cosine between the ray and the normal is less than 0 then
    // the ray intersected the object from the inside and the shading normal
    // should be symmetric to the geometric normal
    Vector3D &shadingNormal(
            (ray.direction_.dotProduct(intersection.normal_) < 0.0f) ?
            intersection.normal_// entering the object
            // We have to reverse the normal now
                                                                     : intersection.getSymNormal());
    // shadowed direct lighting - only for diffuse materials
    if (kD.isNotZero() && (ray.depth_ < RAY_DEPTH_MAX)) {
        const unsigned long sizeLights(scene_.lights_.size());
        Intersection intersectLight;

        for (unsigned int i(0); i < sizeLights; i++)//for each light
        {
            const Light &light(*scene_.lights_[i]);
            //calculates vector starting in intersection to the light
            Vector3D vectorIntersectCamera(light.position_, intersection.point_);
            //distance from intersection to the light (and normalize it)
            const float distanceToLight(vectorIntersectCamera.normalize());
            const float cos_N_L(vectorIntersectCamera.dotProduct(shadingNormal));//x*x + y*y + z*z
            if (cos_N_L > 0.0f) {
                //shadow ray - orig=intersection, dir=light
                const Ray shadowRay(intersection.point_, vectorIntersectCamera, distanceToLight,
                                    ray.depth_ + 1);
                //intersection between shadow ray and the closest primitive
                //if there are no primitives between intersection and the light
                if (!scene_.shadowTrace(intersectLight, shadowRay)) {
                    rgb.add(kD, light.radiance_, cos_N_L);//rgb += kD * radLight * cos_N_L;
                }
            }
        }
        // ambient light
        rgb.add(kD, 0.1f);//rgb += kD *  0.1

        const float fi(sampler_.getSample(0) * 2 * PI);
        const float teta(sampler_.getSample(0) * PI_2);
        //LOG("fi=%f, teta=%f", double(fi), double(teta));
        const float x(std::cos(fi) * std::sin(teta));
        const float y(std::sin(fi) * std::sin(teta));
        const float z(std::cos(teta));
        //angleX = orientation relative to the x axis
        //cos α = 	a · b / |a| · |b|
        //a·b = x1 · x2 + y1 · y2 + z1 · z2
        //|a| = √(x + y + z)
        const float vectorX1(0);
        const float vectorY1(1);
        const float vectorZ1(0);
        const float a(std::sqrt(intersection.normal_.x_ + intersection.normal_.y_ +
                                intersection.normal_.z_));
        const float b1(std::sqrt(vectorX1 + vectorY1 + vectorZ1));
        const float ab1(intersection.normal_.x_ * vectorX1 + intersection.normal_.y_ * vectorY1 +
                        intersection.normal_.z_ * vectorZ1);
        const float angleX(std::acos(ab1 / (a * b1)));
        const float vectorX2(1);
        const float vectorY2(0);
        const float vectorZ2(0);
        const float b2(std::sqrt(vectorX2 + vectorY2 + vectorZ2));
        const float ab2(intersection.normal_.x_ * vectorX2 + intersection.normal_.y_ * vectorY2 +
                        intersection.normal_.z_ * vectorZ2);
        const float angleY(std::acos(ab2 / (a * b2)));
        const float globalX(x * std::cos(angleX) - y * std::sin(angleX) + intersection.point_.x_);
        const float globalY(x * std::sin(angleX) + y * std::cos(angleX) + intersection.point_.y_);
        const float globalZ(
                z * std::cos(angleY) - globalY * std::sin(angleY) + intersection.point_.z_);
        Ray newRay(globalX, globalY, globalZ, intersection.point_, ray.depth_ + 1);
        RGB newRGB;
        Intersection newIntersection;
        rayTrace(newRGB, newRay, newIntersection);
        newRGB *= kD;
        rgb.add(newRGB, 0.1f);

    } // end direct + ambient

    // specular reflection
    const RGB &kS(intersection.material_->Ks_);
    if (kS.isNotZero() && (ray.depth_ < RAY_DEPTH_MAX)) {
        // compute specular reflection
        //reflection ray
        const float RN_dot(2.0f * shadingNormal.dotProduct(ray.symDirection_));
        shadingNormal.mult(RN_dot);
        shadingNormal.subAndNormalize(ray.symDirection_);

        Ray specRay(intersection.point_, shadingNormal, RAY_LENGTH_MAX, ray.depth_ + 1);
        RGB specRad;
        Intersection aux;
        rayTrace(specRad, specRay, aux);
        specRad *= kS;
        rgb.add(specRad);
    }
}
