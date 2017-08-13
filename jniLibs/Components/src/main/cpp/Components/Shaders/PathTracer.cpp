//
// Created by puscas on 20-02-2017.
//

#include "PathTracer.hpp"

using Components::PathTracer;
using MobileRT::Light;
using MobileRT::Vector3D;
using MobileRT::Point3D;
using MobileRT::Sampler;
using MobileRT::RGB;
using MobileRT::Intersection;
using MobileRT::Ray;
using MobileRT::Scene;

PathTracer::PathTracer(Scene &&scene,
                       std::unique_ptr<Sampler> &&samplerRay,
                       std::unique_ptr<Sampler> &&samplerLight,
                       std::unique_ptr<Sampler> &&samplerRussianRoulette,
                       const unsigned samplesLight) noexcept :
  Shader {std::move (scene), samplesLight},
  samplerRay_ {std::move (samplerRay)},
  samplerLight_ {std::move (samplerLight)},
  samplerRussianRoulette_ {std::move (samplerRussianRoulette)} {
    LOG("samplesLight = ", this->samplesLight_);
}

//pag 28 slides Monte Carlo
bool PathTracer::shade (RGB *const rgb, Intersection intersection, Ray &&ray) const noexcept
{
  const unsigned rayDepth {ray.depth_};
  if (rayDepth > RayDepthMax || intersection.material_ == nullptr) {
			return false;
  }

  const RGB &Le {intersection.material_->Le_};
  if (Le.hasColor()) {//stop if it intersects a light source
    //rgb.add(Le * scene_.lights_.size() * RayDepthMax);
    *rgb += Le;
    return true;
  }
  bool intersectedLight {false};
  RGB Ld {};
  RGB LiD {};
  RGB LiS {};
  RGB LiT {};

  const RGB &kD(intersection.material_->Kd_);
  const RGB &kS(intersection.material_->Ks_);
  const RGB &kT(intersection.material_->Kt_);
  static const float finish_probability {0.5f};
  static const float continue_probability {1.0f - finish_probability};

  // the normal always points to outside objects (e.g., spheres)
  // if the cosine between the ray and the normal is less than 0 then
  // the ray intersected the object from the inside and the shading normal
  // should be symmetric to the geometric normal
  const Vector3D &shadingNormal {
    (ray.direction_.dotProduct (intersection.normal_) < 0.0f) ?
    intersection.normal_ :// entering the object
    intersection.symNormal_};// We have to reverse the normal now

    // shadowed direct lighting - only for diffuse materials
    //Ld = Ld (p->Wr)
    if (kD.hasColor()) {
      const uint64_t sizeLights {scene_.lights_.size ()};
        if (sizeLights > 0) {
          const unsigned samplesLight {this->samplesLight_};
          Intersection intersectLight {};
            //direct light
          for (unsigned i {0}; i < samplesLight; i++) {
            const float randomNumber {samplerLight_->getSample ()};
                //PDF = 1 / sizeLights
            const unsigned chosenLight {
              static_cast<unsigned> (std::floor (
                          randomNumber * sizeLights * 0.99999f))};
            Light &light (*scene_.lights_[chosenLight]);
                //calculates vector starting in intersection to the light
            const Point3D lightPosition {light.getPosition ()};
            Vector3D vectorToLight {lightPosition, intersection.point_};
                //distance from intersection to the light (and normalize it)
            const float distanceToLight {vectorToLight.normalize ()};
                //x*x + y*y + z*z
            const float cosNormalLight {shadingNormal.dotProduct (vectorToLight)};
                if (cosNormalLight > 0.0f) {
                    //shadow ray->orig=intersection, dir=light
                  Ray shadowRay {vectorToLight, intersection.point_, rayDepth + 1};
                    //intersection between shadow ray and the closest primitive
                    //if there are no primitives between intersection and the light
                    intersectLight.length_ = distanceToLight;
                    if (!scene_.shadowTrace(&intersectLight, std::move(shadowRay))) {
                        //Ld += kD * radLight * cosNormalLight * sizeLights / samplesLight
                      Ld.addMult ({light.radiance_.Le_}, cosNormalLight);
                    }
                }
            }
            Ld *= kD;
            //Ld *= sizeLights;
            Ld /= samplesLight;
        }

        //indirect light
      if (rayDepth <= RayDepthMin || samplerRussianRoulette_->getSample () > finish_probability) {
        const float r1 {2.0f * Pi * samplerRay_->getSample ()};
        const float r2 {samplerRay_->getSample ()};
        const float r2s {std::sqrt (r2)};
        Vector3D u {std::fabs (intersection.normal_.x_) > 0.1f ?
                    Vector3D (0.0f, 1.0f, 0.0f).crossProduct (intersection.normal_) :
                    Vector3D (1.0f, 0.0f, 0.0f).crossProduct (intersection.normal_)};
        u.normalize();
        const Vector3D aux {intersection.normal_.crossProduct (u)};
        Vector3D direction {(u * std::cos (r1) * r2s + aux * std::sin (r1) * r2s +
                             intersection.normal_ * std::sqrt (1.0f - r2))};
        direction.normalize();
        Ray normalizedSecundaryRay {direction, intersection.point_, rayDepth + 1};

        //Li = Pi/N * SOMATORIO i=1->i=N [fr (p,Wi <-> Wr) L(p <- Wi)]
            //estimator = <F^N>=1/N * ∑(i=0)(N−1) f(Xi) / pdf(Xi)
        RGB LiD_RGB {};
        Intersection secundaryIntersection {};
        intersectedLight = rayTrace(&LiD_RGB, &secundaryIntersection, std::move(normalizedSecundaryRay));
        //PDF = cos(theta) / Pi
        //cos (theta) = cos(dir, normal)
        //PDF = cos(dir, normal) / Pi
        //LiD += kD * LiD_RGB * cos (dir, normal) / (PDF * continue_probability)
        //LiD += kD * LiD_RGB * Pi / continue_probability
        //LiD.addMult(kD, LiD_RGB, Pi);
        LiD.addMult ({kD, LiD_RGB});
        if (rayDepth > RayDepthMin) {
          LiD /= continue_probability;
        }
        //if it has Ld and if LiD intersects a light source then LiD = 0
        if (Ld.hasColor() && intersectedLight) {
          LiD.reset();
          // LiS.reset();
          // LiT.reset();
        }
      }
    }

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
      LiS.addMult ({kS, LiS_RGB});
    }

    // specular transmission
    if (kT.hasColor()) {
      //PDF = 1 / 2 Pi
      Vector3D shadingNormalT {shadingNormal};
      float refractiveIndice {intersection.material_->refractiveIndice_};
        if (shadingNormalT.dotProduct(ray.direction_) > 0.0f) {//we are inside the medium
            shadingNormalT *= -1.0f;//N = N*-1;
            refractiveIndice = 1.0f / refractiveIndice;//n = 1 / n;
        }
        refractiveIndice = 1.0f / refractiveIndice;

      const float cosTheta1 {(shadingNormalT.dotProduct (ray.direction_)) * - 1.0f};
      const float cosTheta2 {
        1.0f - refractiveIndice * refractiveIndice * (1.0f - cosTheta1 * cosTheta1)};
      Ray transmissionRay {cosTheta2 > 0.0f ? // refraction direction
                                  //rayDir = ((ray.d*n) + (N*(n*cost1 - sqrt(cost2)))).norm();
                                  (ray.direction_ * refractiveIndice) +
                                  (shadingNormalT * (refractiveIndice * cosTheta1 -
                                                     (std::sqrt(cosTheta2)))) :
                                  //rayDir = (ray.d + N*(cost1 * 2)).norm();
                                  ray.direction_ + shadingNormalT * (cosTheta1 * 2.0f),
                           intersection.point_,
                           rayDepth + 1};
      RGB LiT_RGB {};
      Intersection transmissionInt {};
        rayTrace(&LiT_RGB, &transmissionInt, std::move(transmissionRay));
      LiT.addMult ({kT, LiT_RGB});
    }
	//if (Ld.hasColor()) {LiD.reset();LiS.reset();LiT.reset();}
    //Ld /= rayDepth;
	//LiD /= rayDepth;

	*rgb += Ld;
	*rgb += LiD;
	*rgb += LiS;
	*rgb += LiT;
	return intersectedLight;
}

void PathTracer::resetSampling() noexcept {
    this->scene_.resetSampling();
    this->samplerRay_->resetSampling();
    this->samplerLight_->resetSampling();
}
