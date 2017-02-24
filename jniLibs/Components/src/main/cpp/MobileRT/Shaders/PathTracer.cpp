//
// Created by puscas on 20-02-2017.
//

#include "PathTracer.h"

using namespace MobileRT;

#define MAX_DEPTH 4

PathTracer::PathTracer(const Scene &scene, Sampler &sampler) :
        Shader(scene), sampler_(sampler) {
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
    if (kD.isNotZero() && (ray.depth_ < MAX_DEPTH)) {
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
        rgb.add(kD, 0.1f);//rgb += kD *  0.1f
    } // end direct + ambient

    Vector3D randomDirection(sampler_.getSample(1, 0), sampler_.getSample(1, 0),
                             sampler_.getSample(1, 0));
    Ray specRay(intersection.point_, randomDirection, RAY_LENGTH_MAX, ray.depth_ + 1);
    RGB specRad;
    Intersection aux;

    /*specRad *= kD;
    rgb.add(specRad);*/

    // specular reflection
    /*const RGB &kS(intersection.material_->Ks_);
    if (kS.isNotZero() && (ray.depth_ < MAX_DEPTH)) {
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
    }*/
}
