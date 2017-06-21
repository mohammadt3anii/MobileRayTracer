//
// Created by Tiago on 16-10-2016.
//

#include "NoShadows.h"

using namespace Components;

NoShadows::NoShadows(Scene &scene, const unsigned int samplesLight) :
        Shader(scene, samplesLight) {
}

void NoShadows::shade(RGB &rgb, Intersection &intersection, Ray &) const {
    const RGB &Le(intersection.material_->Le_);
    const RGB &kD(intersection.material_->Kd_);

    // direct lighting - only for diffuse materials
    if (kD.isNotZero()) {
        const unsigned long sizeLights(scene_.lights_.size());
        const unsigned int samplesLight(this->samplesLight_);
        for (unsigned int i(0); i < sizeLights; i++) {
            for (unsigned int j(0); j < samplesLight; j++) {
                Light &light(*scene_.lights_[i]);
                const Point3D lightPosition(light.getPosition());
                //vectorIntersectCameraNormalized = light.position_ - intersection.point_
                Vector3D vectorToLightNormalized(
                        lightPosition, intersection.point_, true);
                const float cos_N_L(vectorToLightNormalized.dotProduct(
                        intersection.normal_));
                if (cos_N_L > 0.0f) {
                    rgb.addMult(kD, light.radiance_.Le_, cos_N_L);//rgb += kD * radLight * cos_N_L;
                }
            }
        }
        rgb /= this->samplesLight_;
        rgb /= sizeLights;
        // ambient light
        rgb.addMult(kD, 0.1f);//rgb += kD *  0.1f
    } // end direct + ambient

    rgb.add(Le);
}
