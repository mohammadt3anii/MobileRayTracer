//
// Created by Tiago on 16-10-2016.
//

#include "NoShadows.h"

using namespace MobileRT;

NoShadows::NoShadows(const Scene &scene) : Shader(scene) {
}

void NoShadows::shade(RGB &rgb, Intersection &intersection, const Ray &,
                      Vector3D &vectorIntersectCamera,
                      RayTraceCall) const {
    const RGB &kD(intersection.material_->Kd_);
    rgb.reset();

    // direct lighting - only for diffuse materials
    if (!kD.isZero()) {
        const unsigned int Nl(static_cast<unsigned int> (scene_.lights.size()));

        for (unsigned int l(0); l < Nl; l++) {
            const PointLight *light(scene_.lights[l]);

            vectorIntersectCamera.reset(light->position_, intersection.point_);
            vectorIntersectCamera.normalize();
            const float cos_N_L(vectorIntersectCamera.dotProduct(intersection.normal_));
            if (cos_N_L > 0.0f) {
                const RGB &radLight(light->radiance_);
                rgb.R_ += kD.R_ * cos_N_L * radLight.R_;
                rgb.G_ += kD.G_ * cos_N_L * radLight.G_;
                rgb.B_ += kD.B_ * cos_N_L * radLight.B_;
            }
        }
        // ambient light
        rgb.R_ += kD.R_ * 0.1f;
        rgb.G_ += kD.G_ * 0.1f;
        rgb.B_ += kD.B_ * 0.1f;
    } // end direct + ambient
}