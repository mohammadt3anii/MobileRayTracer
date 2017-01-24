//
// Created by Tiago on 16-10-2016.
//

#include "Whitted.h"

using namespace MobileRT;

Whitted::Whitted(const Scene &scene) : Shader(scene) {
}

void Whitted::shade(RGB &rgb,
                    Intersection &intersection,
                    const Ray &ray,
                    Vector3D &vectorIntersectCamera) const {
    // the normal always points to outside objects (e.g., spheres)
    // if the cosine between the ray and the normal is less than 0 then
    // the ray intersected the object from the inside and the shading normal
    // should be symmetric to the geometric normal
    Vector3D &shadingNormal((ray.direction_.dotProduct(intersection.normal_) < 0.0f) ?
                            intersection.normal_// entering the object
                                                                                     : intersection.getSymNormal());// We have to reverse the normal now

    rgb.reset();
    // shadowed direct lighting - only for diffuse materials
    if (!intersection.material_->Kd_.isZero()) {
        const unsigned long sizeLights(scene_.lights.size());
        Intersection intersectLight;

        for (unsigned int i(0); i < sizeLights; i++)//for each light
        {
            const Light *const light(scene_.lights[i]);

            //calculates vector starting in intersection to the light
            vectorIntersectCamera.reset(light->position_, intersection.point_);
            //distance from intersection to the light (and normalize it)
            const float distanceToLight(vectorIntersectCamera.normalize());
            const float cos_N_L(vectorIntersectCamera.dotProduct(shadingNormal));//x*x + y*y + z*z
            if (cos_N_L > 0.0f) {
                //shadow ray - orig=intersection, dir=light
                const Ray shadowRay(intersection.point_, vectorIntersectCamera, distanceToLight,
                                    ray.depth_ + 1);
                //intersection between shadow ray and the closest primitive
                //intersectLight = ();
                //if there are no primitives between intersection and the light
                if (!scene_.shadowTrace(intersectLight, shadowRay)) {
                    RGB diffuseRad(light->radiance_);//R=1, G=1, B=1
                    diffuseRad.mult(intersection.material_->Kd_);//cor da luz
                    diffuseRad.mult(cos_N_L);//angle to normal
                    rgb.add(diffuseRad);//adds the light color
                }
            }
        }
        // ambient light
        rgb.R_ += intersection.material_->Kd_.R_ * 0.1f;
        rgb.G_ += intersection.material_->Kd_.G_ * 0.1f;
        rgb.B_ += intersection.material_->Kd_.B_ * 0.1f;
    } // end direct + ambient
    // specular reflection
    if (!intersection.material_->Ks_.isZero() && (ray.depth_ < this->MAX_DEPTH)) {
        // compute specular reflection
        //reflection ray
        const float RN_dot(2.0f * shadingNormal.dotProduct(ray.symDirection_));
        shadingNormal.mult(RN_dot);
        shadingNormal.sub(ray.symDirection_);
        shadingNormal.normalize();

        Ray specRay(intersection.point_, shadingNormal, RAY_LENGTH_MAX, ray.depth_ + 1);
        RGB specRad;
        Intersection aux;
        rayTrace(specRad, specRay, aux, vectorIntersectCamera);
        specRad.mult(intersection.material_->Ks_);
        rgb.add(specRad);
    }
}