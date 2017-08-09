//
// Created by Tiago on 16-10-2016.
//

#include "NoShadows.hpp"

using Components::NoShadows;
using MobileRT::Light;
using MobileRT::Point3D;
using MobileRT::Vector3D;
using MobileRT::RGB;
using MobileRT::Intersection;
using MobileRT::Ray;
using MobileRT::Scene;

NoShadows::NoShadows(Scene &&scene, const unsigned int samplesLight) noexcept :
  Shader {std::move (scene), samplesLight} {
}

bool NoShadows::shade (RGB *const rgb, Intersection intersection, Ray &&ray) const noexcept {

  const RGB &Le {intersection . material_ -> Le_};
  const RGB &kD {intersection . material_ -> Kd_};

    if (Le.hasColor()) {//stop if it intersects a light source
        *rgb += Le;
        return true;
    }

  const Vector3D &shadingNormal {
    (ray . direction_ . dotProduct (intersection . normal_) < 0.0f) ?
            // entering the object
            intersection.normal_ :
            // We have to reverse the normal now
    intersection . symNormal_};

    // direct lighting - only for diffuse materials
    if (kD.hasColor()) {
      const uint64_t sizeLights {scene_ . lights_ . size ()};
      const unsigned int samplesLight {this -> samplesLight_};
      for (unsigned int i {0}; i < sizeLights; i ++) {
        Light &light (*scene_ . lights_[i]);
        for (unsigned int j {0}; j < samplesLight; j ++) {
          const Point3D lightPosition {light . getPosition ()};
                //vectorIntersectCameraNormalized = light.position_ - intersection.point_
          const Vector3D vectorToLightNormalized {
            lightPosition, intersection . point_, true};
          const float cos_N_L {shadingNormal . dotProduct (vectorToLightNormalized)};
                if (cos_N_L > 0.0f) {
                    //rgb += kD * radLight * cos_N_L;
                  rgb->addMult ({light.radiance_.Le_}, {cos_N_L});
                }
            }
        }
        *rgb *= kD;
        *rgb /= this->samplesLight_;
    } // end direct + ambient

  rgb->addMult ({kD}, {0.1f});//rgb += kD *  0.1f
    return false;
}

void NoShadows::resetSampling() noexcept {
    this->scene_.resetSampling();
}
