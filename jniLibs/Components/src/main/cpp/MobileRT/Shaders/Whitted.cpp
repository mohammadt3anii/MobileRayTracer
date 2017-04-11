//
// Created by Tiago on 16-10-2016.
//

#include "Whitted.h"

using namespace MobileRT;

Whitted::Whitted(Scene &scene, const unsigned int samplesLight) :
        Shader(scene, samplesLight) {
}

void Whitted::shade(RGB &rgb, Intersection &intersection, Ray &ray) const {
    if (!intersection.material_ || ray.depth_ > RAY_DEPTH_MAX) return;

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
            // We have to reverse the normal now
                                                                     : intersection.getSymNormal());

    // shadowed direct lighting - only for diffuse materials
    if (kD.isNotZero()) {
        const unsigned long sizeLights(scene_.lights_.size());
        Intersection lightIntersection;
        //LOG("samplesLight_ = %u", this->samplesLight_);
        const unsigned int samplesLight(static_cast<unsigned int> (std::sqrt(this->samplesLight_)));

        for (unsigned int i(0); i < sizeLights; i++)//for each light
        {
            Light &light(*scene_.lights_[i]);
            RGB lightRGB;
            light.resetSampling();
            for (unsigned int j(0); j < samplesLight; j++) {
                for (unsigned int k(0); k < samplesLight; k++) {
                    Point3D lightPosition(light.getPosition(j, k));
                    //calculates vector starting in intersection to the light
                    Vector3D vectorToLight(lightPosition, intersection.point_);
                    //distance from intersection to the light (and normalize it)
                    const float distanceToLight(vectorToLight.normalize());
                    const float cos_N_L(shadingNormal.dotProduct(vectorToLight));
                    if (cos_N_L > 0.0f) {
                        //shadow ray - orig=intersection, dir=light
                        const Ray shadowRay(intersection.point_, vectorToLight,
                                            distanceToLight, ray.depth_ + 1);
                        //intersection between shadow ray and the closest primitive
                        //if there are no primitives between intersection and the light
                        if (!scene_.shadowTrace(lightIntersection, shadowRay)) {
                            //rgb += kD * radLight * cos_N_L;
                            lightRGB.add(kD, light.radiance_, cos_N_L);
                        }
                    }
                }
            }
            lightRGB /= this->samplesLight_;
            rgb.add(lightRGB);
        }
        rgb /= sizeLights;
        // ambient light
        rgb.add(kD, 0.1f);//rgb += kD *  0.1f
    } // end direct + ambient

    // specular reflection
    if (kS.isNotZero()) {
        // compute specular reflection
        //reflection ray
        Vector3D &shadingNormalR(shadingNormal);
        const float RN_dot(2.0f * shadingNormalR.dotProduct(ray.symDirection_));
        shadingNormalR.mult(RN_dot);
        shadingNormalR.subAndNormalize(ray.symDirection_);//incident + 2 * cosI * normal

        Ray specularRay(intersection.point_, shadingNormalR, RAY_LENGTH_MAX, ray.depth_ + 1);
        RGB specularRad;
        Intersection specularInt;
        rayTrace(specularRad, specularRay, specularInt);
        specularRad *= kS;
        rgb.add(specularRad);
    }

    // specular transmission
    if (kT.isNotZero()) {
        // compute specular transmission
        //transmission ray
        //Vector3D &shadingNormalT(shadingNormal);
        /*Vector3D &shadingNormalT(ray.direction_);
        const float RN_dot(1.0f - 2.0f * shadingNormalT.dotProduct(ray.symDirection_));*/

        //sin2(x) = 1 - cos2(x)
        /*const float sinTheta1 = 1.0f * std::sqrt(1.0f - RN_dot*RN_dot);
        //fresnel equation -> n1 * sin(theta1) = n2 * sin(theta2)
        const float sinTheta2 = sinTheta1 / intersection.refractiveIndice_;
        const float angle (std::asin (sinTheta2));*/
        //LOG("angle = %f", double(angle));

        /*shadingNormalT.mult(RN_dot);
        shadingNormalT.subAndNormalize(ray.symDirection_);

        Ray transmissionRay(intersection.point_, shadingNormalT, RAY_LENGTH_MAX, ray.depth_ + 1);
        RGB transmissionRad;
        Intersection transmissionInt;
        rayTrace(transmissionRad, transmissionRay, transmissionInt);
        transmissionRad *= kT;
        rgb.add(transmissionRad);*/


        //double n = params["refr_index"];
        //double R0 = (1.0 - n) / (1.0 + n);
        //R0 = R0*R0;
        float refractiveIndice(intersection.material_->refractiveIndice_);
        //float R0 = (1.0f - refractiveIndice) / (1.0f + refractiveIndice);
        //R0 = R0*R0;
        if (intersection.normal_.dotProduct(ray.direction_) > 0) { // we're inside the medium
            //N = N*-1;
            //n = 1 / n;
            intersection.normal_.mult(-1.0f);
            refractiveIndice = 1.0f / refractiveIndice;
        }
        refractiveIndice = 1.0f / refractiveIndice;

        //double cost1 = (N.dot(ray.d))*-1; // cosine of theta_1
        //double cost2 = 1.0 - n*n*(1.0 - cost1*cost1); // cosine of theta_2
        //double Rprob = R0 + (1.0 - R0) * pow(1.0 - cost1, 5.0); // Schlick-approximation
        const float cost1((intersection.normal_.dotProduct(ray.direction_)) * -1);
        const float cost2(1.0f - refractiveIndice * refractiveIndice * (1.0f - cost1 * cost1));
        //const float Rprob (R0 + (1.0f - R0) * std::pow(1.0f - cost1, 5.0f));
        Ray transmissionRay;
        RGB transmissionRad;
        Intersection transmissionInt;
        if (cost2 > 0 /*&& RND2 > Rprob*/) { // refraction direction
            //ray.d = ((ray.d*n) + (N*(n*cost1 - sqrt(cost2)))).norm();
            transmissionRay.direction_ = ((ray.direction_ * refractiveIndice) +
                                          (intersection.normal_ * (refractiveIndice * cost1 -
                                                                   static_cast<float> (std::sqrt(
                                                                           cost2)))));
        } else {// reflection direction
            //ray.d = (ray.d + N*(cost1 * 2)).norm();
            transmissionRay.direction_ = (ray.direction_ + intersection.normal_ * (cost1 * 2));
        }
        transmissionRay.direction_.normalize();
        transmissionRay.depth_ = ray.depth_ + 1;
        transmissionRay.origin_ = intersection.point_;
        transmissionRay.calcSymDirection();
        rayTrace(transmissionRad, transmissionRay, transmissionInt);
        //trace(ray, scene, depth + 1, tmp, params, hal, hal2);
        //clr = clr + tmp * 1.15 * rrFactor;
        transmissionRad *= kT;
        rgb.add(transmissionRad);
    }
}
