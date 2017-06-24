//
// Created by Tiago on 16-10-2016.
//

#include "Whitted.h"

using namespace Components;

Whitted::Whitted(Scene &scene, const unsigned int samplesLight) :
        Shader(scene, samplesLight) {
}

void Whitted::shade(RGB &rgb, Intersection &intersection, const Ray &ray) const {
    const unsigned int rayDepth(ray.depth_);
    if (rayDepth > RAY_DEPTH_MAX) return;

    const RGB &Le(intersection.material_->Le_);
    if (Le.isNotZero())//stop if it intersects a light source
    {
        rgb.add(Le);
        return;
    }

    const RGB &kD(intersection.material_->Kd_);
    const RGB &kS(intersection.material_->Ks_);
    const RGB &kT(intersection.material_->Kt_);

    // the normal always points to outside objects (e.g., spheres)
    // if the cosine between the ray and the normal is less than 0 then
    // the ray intersected the object from the inside and the shading normal
    // should be symmetric to the geometric normal
    Vector3D &shadingNormal(
            (ray.direction_.dotProduct(intersection.normal_) < 0.0f) ?
            intersection.normal_// entering the object
                                                                     : intersection.symNormal_);// We have to reverse the normal now

    // shadowed direct lighting - only for diffuse materials
    if (kD.isNotZero()) {
        const unsigned long sizeLights(scene_.lights_.size());
        Intersection lightIntersection;
        const unsigned int samplesLight(this->samplesLight_);
        for (unsigned int i(0); i < sizeLights; i++) {
            Light &light(*scene_.lights_[i]);
            RGB lightRGB;
            for (unsigned int j(0); j < samplesLight; j++) {
                const Point3D lightPosition(light.getPosition());
                //calculates vector starting in intersection to the light
                Vector3D vectorToLight(lightPosition, intersection.point_);
                //distance from intersection to the light (and normalize it)
                const float distanceToLight(vectorToLight.normalize());
                const float cos_N_L(shadingNormal.dotProduct(vectorToLight));
                if (cos_N_L > 0.0f) {
                    //shadow ray - orig=intersection, dir=light
                    const Ray shadowRay(vectorToLight, intersection.point_, rayDepth + 1u);
                    lightIntersection.length_ = distanceToLight;
                    //intersection between shadow ray and the closest primitive
                    //if there are no primitives between intersection and the light
                    if (!scene_.shadowTrace(lightIntersection, shadowRay)) {
                        //rgb += kD * radLight * cos_N_L;
                        lightRGB.addMult(light.radiance_.Le_, cos_N_L);
                    }
                }
            }
            lightRGB *= kD;
            lightRGB /= this->samplesLight_;
            rgb.add(lightRGB);
        }
        // ambient light
    } // end direct + ambient

    // specular reflection
    if (kS.isNotZero()) {
        //PDF = 1 / 2 PI

        //reflectionDir = rayDirection - (2 * rayDirection . normal) * normal
        const Vector3D reflectionDir(
                ray.direction_, shadingNormal, 2.0f * shadingNormal.dotProduct(ray.direction_));

        const Ray specularRay(reflectionDir, intersection.point_, rayDepth + 1u);
        RGB LiS_RGB;
        Intersection specularInt;
        rayTrace(LiS_RGB, specularRay, specularInt);
        rgb.addMult(kS, LiS_RGB);
    }

    // specular transmission
    if (kT.isNotZero()) {
        //PDF = 1 / 2 PI

        float refractiveIndice(intersection.material_->refractiveIndice_);
        if (shadingNormal.dotProduct(ray.direction_) > 0.0f) {//we are inside the medium
            shadingNormal.mult(-1.0f);//N = N*-1;
            refractiveIndice = 1.0f / refractiveIndice;//n = 1 / n;
        }
        refractiveIndice = 1.0f / refractiveIndice;

        const float cosTheta1((shadingNormal.dotProduct(ray.direction_)) * -1.0f);
        const float cosTheta2(
                1.0f - refractiveIndice * refractiveIndice * (1.0f - cosTheta1 * cosTheta1));
        const Ray transmissionRay(cosTheta2 > 0.0f ? // refraction direction
                                  //rayDir = ((ray.d*n) + (N*(n*cost1 - sqrt(cost2)))).norm();
                                  (ray.direction_ * refractiveIndice) +
                                  (shadingNormal * (refractiveIndice * cosTheta1 -
                                                    (std::sqrt(cosTheta2)))) :
                                  //rayDir = (ray.d + N*(cost1 * 2)).norm();
                                  ray.direction_ + shadingNormal * (cosTheta1 * 2.0f),
                                  intersection.point_,
                                  rayDepth + 1u);
        RGB LiT_RGB;
        Intersection transmissionInt;
        rayTrace(LiT_RGB, transmissionRay, transmissionInt);
        rgb.addMult(kT, LiT_RGB);
    }

    rgb.addMult(kD, 0.1f);//rgb += kD *  0.1f
}
