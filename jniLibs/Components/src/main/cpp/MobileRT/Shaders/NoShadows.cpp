//
// Created by Tiago on 16-10-2016.
//

#include "NoShadows.h"

using namespace MobileRT;

NoShadows::NoShadows(Scene &scene, const unsigned int samplesLight) :
        Shader(scene, samplesLight) {
}

void NoShadows::shade(RGB &rgb, Intersection &intersection, Ray &) const {
    if (!intersection.material_) return;
    const RGB &kD(intersection.material_->Kd_);

    // direct lighting - only for diffuse materials
    if (kD.isNotZero()) {
        const unsigned long sizeLights(scene_.lights_.size());
        const unsigned int samplesLight(static_cast<unsigned int> (std::sqrt(this->samplesLight_)));
        for (unsigned int i(0); i < sizeLights; i++) {
            for (unsigned int j(0); j < samplesLight; j++) {
                for (unsigned int k(0); k < samplesLight; k++) {
                    Light &light(*scene_.lights_[i]);
                    Point3D lightPosition(light.getPosition(j, k));
                    //vectorIntersectCameraNormalized = light.position_ - intersection.point_
                    Vector3D vectorIntersectCameraNormalized(
                            lightPosition, intersection.point_, true);
                    const float cos_N_L(vectorIntersectCameraNormalized.dotProduct(
                            intersection.normal_));
                    if (cos_N_L > 0.0f) {
                        rgb.add(kD, light.radiance_, cos_N_L);//rgb += kD * radLight * cos_N_L;
                    }
                }
            }
        }
        rgb /= this->samplesLight_;
        // ambient light
        rgb.add(kD, 0.1f);//rgb += kD *  0.1f
    } // end direct + ambient
}
