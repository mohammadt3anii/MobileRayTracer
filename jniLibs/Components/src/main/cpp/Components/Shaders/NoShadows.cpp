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

NoShadows::NoShadows (Scene &&scene, const unsigned samplesLight,
                      const Accelerator accelerator) noexcept :
  Shader {std::move (scene), samplesLight, accelerator} {
}

bool NoShadows::shade (RGB *const rgb, const Intersection intersection, Ray &&ray) noexcept {
  const RGB &Le {intersection.material_->Le_};
  const RGB &kD {intersection.material_->Kd_};
  if (Le.hasColor ()) {//stop if it intersects a light source
    *rgb += Le;
    return true;
  }

  const Vector3D &shadingNormal {
    (ray.direction_.dotProduct (intersection.normal_) < 0.0f) ?
            // entering the object
            intersection.normal_ :
            // We have to reverse the normal now
    intersection.symNormal_};

  // direct lighting - only for diffuse materials
  if (kD.hasColor ()) {
    const uint64_t sizeLights {scene_.lights_.size ()};
    if (sizeLights > 0) {
      const unsigned samplesLight {this->samplesLight_};
      for (unsigned i {0}; i < sizeLights; i++) {
        Light &light (*scene_.lights_[i]);
        for (unsigned j {0}; j < samplesLight; j++) {
          const Point3D lightPosition {light.getPosition ()};
          //vectorIntersectCameraNormalized = light.position_ - intersection.point_
          const Vector3D vectorToLightNormalized {
            lightPosition, intersection.point_, true};
          const float cos_N_L {shadingNormal.dotProduct (vectorToLightNormalized)};
          if (cos_N_L > 0.0f) {
            //rgb += kD * radLight * cos_N_L;
            rgb->addMult ({light.radiance_.Le_}, cos_N_L);
          }
        }
      }
      *rgb *= kD;
      *rgb /= samplesLight;
      *rgb /= sizeLights;
    } // end direct + ambient
  }

  rgb->addMult ({kD}, 0.1f);//rgb += kD *  0.1f
  return false;
}

void NoShadows::resetSampling() noexcept {
    this->scene_.resetSampling();
}
