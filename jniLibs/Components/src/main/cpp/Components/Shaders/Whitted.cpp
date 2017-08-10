//
// Created by Tiago on 16-10-2016.
//

#include "Whitted.hpp"

using Components::Whitted;
using MobileRT::Light;
using MobileRT::Vector3D;
using MobileRT::Point3D;
using MobileRT::RGB;
using MobileRT::Intersection;
using MobileRT::Ray;
using MobileRT::Scene;

Whitted::Whitted(Scene &&scene, const unsigned int samplesLight) noexcept :
  Shader {std::move (scene), samplesLight} {
}

bool Whitted::shade (RGB *const rgb, Intersection intersection, Ray &&ray) const noexcept {

  const unsigned int rayDepth {ray.depth_};
  if (rayDepth > RayDepthMax) {
		return false;
	}

  const RGB &Le {intersection.material_->Le_};
    if (Le.hasColor()) {//stop if it intersects a light source
		*rgb += Le;
		return true;
	}

  const RGB &kD {intersection.material_->Kd_};
  const RGB &kS {intersection.material_->Ks_};
  const RGB &kT {intersection.material_->Kt_};

	// the normal always points to outside objects (e.g., spheres)
	// if the cosine between the ray and the normal is less than 0 then
	// the ray intersected the object from the inside and the shading normal
	// should be symmetric to the geometric normal
  const Vector3D &shadingNormal {
    (ray.direction_.dotProduct (intersection.normal_) < 0.0f) ?
            // entering the object
            intersection.normal_ :
            // We have to reverse the normal now
    intersection.symNormal_};

	// shadowed direct lighting - only for diffuse materials
    if (kD.hasColor()) {
      Intersection lightIntersection {};
      const uint64_t sizeLights {scene_.lights_.size ()};
      const unsigned int samplesLight {this->samplesLight_};
      for (unsigned int i {0}; i < sizeLights; i ++) {
        Light &light (*scene_.lights_[i]);
        for (unsigned int j {0}; j < samplesLight; j ++) {
          const Point3D lightPosition {light.getPosition ()};
				//calculates vector starting in intersection to the light
          Vector3D vectorToLight {lightPosition, intersection.point_};
				//distance from intersection to the light (and normalize it)
          const float distanceToLight {vectorToLight.normalize ()};
          const float cos_N_L {shadingNormal.dotProduct (vectorToLight)};
                if (cos_N_L > 0.0f) {
					//shadow ray - orig=intersection, dir=light
                  Ray shadowRay {vectorToLight, intersection.point_, rayDepth + 1};
					lightIntersection.length_ = distanceToLight;
					//intersection between shadow ray and the closest primitive
					//if there are no primitives between intersection and the light
                  if (! scene_.shadowTrace (&lightIntersection, std::move (shadowRay))) {
						//rgb += kD * radLight * cos_N_L;
                    rgb->addMult ({light.radiance_.Le_}, cos_N_L);
					}
				}
			}
			*rgb *= kD;
			*rgb /= this->samplesLight_;
		}
	} // end direct + ambient

	// specular reflection
    if (kS.hasColor()) {
      //PDF = 1 / 2 Pi

		//reflectionDir = rayDirection - (2 * rayDirection.normal) * normal
      const Vector3D reflectionDir {
        ray.direction_, shadingNormal, 2.0f * shadingNormal.dotProduct (ray.direction_)};
      Ray specularRay {reflectionDir, intersection.point_, rayDepth + 1};
      RGB LiS_RGB {};
      Intersection specularInt {};
		rayTrace(&LiS_RGB, &specularInt, std::move(specularRay));
      rgb->addMult ({kS, LiS_RGB});
	}

	// specular transmission
    if (kT.hasColor()) {
      //PDF = 1 / 2 Pi

      Vector3D shadingNormalT {shadingNormal};
      float refractiveIndice {intersection.material_->refractiveIndice_};
        if (shadingNormalT.dotProduct(ray.direction_) > 0.0f) {
            //we are inside the medium
			shadingNormalT *= -1.0f;									//N = N*-1;
			refractiveIndice = 1.0f / refractiveIndice; //n = 1 / n;
		}
		refractiveIndice = 1.0f / refractiveIndice;

      const float cosTheta1 {(shadingNormalT.dotProduct (ray.direction_)) * - 1.0f};
      const float cosTheta2 {
        1.0f - refractiveIndice * refractiveIndice * (1.0f - cosTheta1 * cosTheta1)};
      Ray transmissionRay {cosTheta2 > 0.0f ?// refraction direction
                            //rayDir = ((ray.d*n) + (N*(n*cost1 - sqrt(cost2)))).norm();
                            (ray.direction_ * refractiveIndice) + (shadingNormalT *
                                                                   (refractiveIndice * cosTheta1 -
                                                                    (std::sqrt(cosTheta2)))) :
                            //rayDir = (ray.d + N*(cost1 * 2)).norm();
                            ray.direction_ + shadingNormalT * (cosTheta1 * 2.0f),
                           intersection.point_,
                           rayDepth + 1};
      RGB LiT_RGB {};
      Intersection transmissionInt {};
		rayTrace(&LiT_RGB, &transmissionInt, std::move(transmissionRay));
      rgb->addMult ({kT, LiT_RGB});
	}
  rgb->addMult ({kD}, 0.1f);//rgb += kD *  0.1f
	return false;
}

void Whitted::resetSampling() noexcept {
    this->scene_.resetSampling();
}
