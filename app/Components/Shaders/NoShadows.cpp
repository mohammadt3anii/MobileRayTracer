//
// Created by Tiago on 16-10-2016.
//

#include "NoShadows.hpp"
#include <glm/glm.hpp>

using ::Components::NoShadows;
using ::MobileRT::Light;
using ::MobileRT::Intersection;
using ::MobileRT::Ray;
using ::MobileRT::Scene;

NoShadows::NoShadows(Scene scene, const unsigned samplesLight,
                     const Accelerator accelerator) noexcept :
        Shader{::std::move(scene), samplesLight, accelerator} {
}

bool NoShadows::shade(glm::vec3 *const rgb, const Intersection intersection, Ray ray) noexcept {
    const glm::vec3 &Le{intersection.material_->Le_};
    const glm::vec3 &kD{intersection.material_->Kd_};
    //stop if it intersects a light source
    if (glm::any(glm::greaterThan(*rgb, glm::vec3(::MobileRT::Epsilon)))) {
        *rgb = Le;
        return true;
    }

    const glm::vec3 &shadingNormal{
            (glm::dot(ray.direction_, intersection.normal_) < 0.0f) ?
            // entering the object
            intersection.normal_ :
            // We have to reverse the normal now
            intersection.symNormal_};

    // direct lighting - only for diffuse materials
    if (glm::any(glm::greaterThan(kD, glm::vec3(::MobileRT::Epsilon)))) {
        const uint64_t sizeLights{scene_.lights_.size()};
        if (sizeLights > 0) {
            const unsigned samplesLight{this->samplesLight_};
            for (unsigned i{0}; i < sizeLights; i++) {
                Light &light(*scene_.lights_[i]);
                for (unsigned j{0}; j < samplesLight; j++) {
                    const glm::vec3 lightPosition{light.getPosition()};
                    //vectorIntersectCameraNormalized = light.position_ - intersection.point_
                    const glm::vec3 vectorToLightNormalized{
                            glm::normalize(lightPosition - intersection.point_)};
                    const float cos_N_L{glm::dot(shadingNormal, vectorToLightNormalized)};
                    if (cos_N_L > 0.0f) {
                        //rgb += kD * radLight * cos_N_L;
                        *rgb += light.radiance_.Le_ * cos_N_L;
                    }
                }
            }
            *rgb *= kD;
            *rgb /= samplesLight;
            *rgb /= sizeLights;
        } // end direct + ambient
    }
    *rgb += kD * 0.1f;
    return false;
}

void NoShadows::resetSampling() noexcept {
    this->scene_.resetSampling();
}
