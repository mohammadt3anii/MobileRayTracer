//
// Created by Tiago on 16-10-2016.
//

#include "NoShadows.h"

using namespace MobileRT;

NoShadows::NoShadows(const Scene &scene) : Shader(scene) {
}

void NoShadows::shade(RGB &rgb, Intersection &intersection, const Ray &) const {
    const RGB &kD(intersection.material_->Kd_);
    // direct lighting - only for diffuse materials
    if (kD.isNotZero()) {
        const unsigned long Nl(scene_.sizeLights_);
        for (unsigned int l(0); l < Nl; l++) {
            const Light &light(*scene_.lights_[l]);
            //vectorIntersectCameraNormalized = light.position_ - intersection.point_
            Vector3D vectorIntersectCameraNormalized(light.position_, intersection.point_, true);
            const float cos_N_L(vectorIntersectCameraNormalized.dotProduct(intersection.normal_));
            if (cos_N_L > 0.0f) {
                rgb.add(kD, light.radiance_, cos_N_L);//rgb += kD * radLight * cos_N_L;
            }
        }
        // ambient light
        rgb.add(kD, 0.1f);
    } // end direct + ambient
}
