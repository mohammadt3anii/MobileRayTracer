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

PathTracer::PathTracer(Scene &scene, Sampler &samplerRay, Sampler &samplerLight,
                       const unsigned int samplesLight) noexcept :
        Shader(scene, samplesLight),
        samplerRay_(samplerRay),
        samplerLight_(samplerLight) {
    LOG("sizeLights = ", scene_.lights_.size());
    LOG("samplesLight = ", this->samplesLight_);
}

//pag 28 slides Monte Carlo
void PathTracer::shade(RGB &rgb, Intersection const &intersection, Ray &&ray) const noexcept
{
	const unsigned int rayDepth(ray.depth_);
	static thread_local unsigned int max(RAY_DEPTH_MIN); 
    if (rayDepth > RAY_DEPTH_MAX || intersection.material_ == nullptr) {return;}
	if (rayDepth > max) {max = rayDepth; LOG("rayDepth = ", rayDepth);}

    const RGB &Le(intersection.material_->Le_);
    if (Le.hasColor())//stop if it intersects a light source
    {
        //rgb.add(Le * scene_.lights_.size() * RAY_DEPTH_MAX);
        rgb += Le;
        return;
    }
	RGB Ld;
	RGB LiD;
	RGB LiS;
	RGB LiT;

    const RGB &kD(intersection.material_->Kd_);
    const RGB &kS(intersection.material_->Ks_);
    const RGB &kT(intersection.material_->Kt_);

    thread_local static std::uniform_real_distribution<float> uniform_dist(0.0f, 1.0f);
    thread_local static std::mt19937 gen(std::random_device{}());
    static const float finish_probability(0.5f);
    static const float continue_probability(1.0f - finish_probability);

    // the normal always points to outside objects (e.g., spheres)
    // if the cosine between the ray and the normal is less than 0 then
    // the ray intersected the object from the inside and the shading normal
    // should be symmetric to the geometric normal
    const Vector3D &shadingNormal(
            (ray.direction_.dotProduct(intersection.normal_) < 0.0f) ?
            intersection.normal_ :// entering the object
            intersection.symNormal_);// We have to reverse the normal now

    // shadowed direct lighting - only for diffuse materials
    //Ld = Ld (p -> Wr)
    if (kD.hasColor()) {
        const uint64_t sizeLights(scene_.lights_.size());
        const unsigned int samplesLight(this->samplesLight_);
        Intersection intersectLight;
        //direct light
        for (unsigned int i(0u); i < samplesLight; i++) {
            const float randomNumber(samplerLight_.getSample(0u));
            //PDF = 1 / sizeLights
            const auto chosenLight(
                    static_cast<unsigned int> ((randomNumber + 0.5f) * (sizeLights - 1ul)));
            Light &light(*scene_.lights_[chosenLight]);
            //calculates vector starting in intersection to the light
            const Point3D lightPosition(light.getPosition());
            const Vector3D vectorToLight(lightPosition, intersection.point_, true);
            //distance from intersection to the light (and normalize it)
            const float distanceToLight(vectorToLight.magnitude_);
            //x*x + y*y + z*z
            const float cosNormalLight(shadingNormal.dotProduct(vectorToLight));
            if (cosNormalLight > 0.0f) {
                //shadow ray -> orig=intersection, dir=light
                Ray shadowRay(vectorToLight, intersection.point_, rayDepth + 1u);
                //intersection between shadow ray and the closest primitive
                //if there are no primitives between intersection and the light
                intersectLight.length_ = distanceToLight;
                if (!scene_.shadowTrace(intersectLight, std::move(shadowRay))) {
                    //Ld += kD * radLight * cosNormalLight * sizeLights / samplesLight
                    Ld.addMult(light.radiance_.Le_, cosNormalLight);
                }
            }
        }
        Ld *= kD;
        //Ld *= sizeLights;
        Ld /= samplesLight;

        //indirect light
        if (rayDepth <= RAY_DEPTH_MIN || uniform_dist(gen) > finish_probability) {
            const float r1(2.0f * PI * samplerRay_.getSample(0u));
            const float r2(samplerRay_.getSample(0u));
            const float r2s(std::sqrt(r2));
            Vector3D u(std::fabs(intersection.normal_.x_) > 0.1f ?
                       Vector3D(0.0f, 1.0f, 0.0f).crossProduct(intersection.normal_) :
                       Vector3D(1.0f, 0.0f, 0.0f).crossProduct(intersection.normal_));
            u.normalize();
            const Vector3D aux(intersection.normal_.crossProduct(u));
            Vector3D direction((u * std::cos(r1) * r2s + aux * std::sin(r1) * r2s +
                                intersection.normal_ * std::sqrt(1.0f - r2)));
            direction.normalize();
            Ray normalizedSecundaryRay(direction, intersection.point_,
                                             rayDepth + 1u);

            //Li = PI/N * SOMATORIO i=1 -> i=N [fr (p,Wi <-> Wr) L(p <- Wi)]
            //estimator = <F^N>=1/N * ∑(i=0)(N−1) f(Xi) / pdf(Xi)
            RGB LiD_RGB;
						Intersection secundaryIntersection(rayTrace(LiD_RGB, std::move(normalizedSecundaryRay)));
						//PDF = cos(theta) / PI
						//cos (theta) = cos(dir, normal)
						//PDF = cos(dir, normal) / PI
						//LiD += kD * LiD_RGB * cos (dir, normal) / (PDF * continue_probability)
						//LiD += kD * LiD_RGB * PI / continue_probability
						//LiD.addMult(kD, LiD_RGB, PI);
						LiD.addMult(kD, LiD_RGB);
						if (rayDepth > RAY_DEPTH_MIN)
						{
							LiD /= continue_probability;}

            //if it has Ld and if LiD intersects a light source then LiD = 0
            if (Ld.hasColor() && secundaryIntersection.material_ != nullptr &&
                !secundaryIntersection.material_->Le_.hasColor()) {
                LiD.reset();
            }
				}
    }

    // specular reflection
    if (kS.hasColor()) {
        //PDF = 1 / 2 PI
        //reflectionDir = rayDirection - (2 * rayDirection . normal) * normal
        const Vector3D reflectionDir(
                ray.direction_, shadingNormal, 2.0f * shadingNormal.dotProduct(ray.direction_));

        Ray specularRay(reflectionDir, intersection.point_, rayDepth + 1u);
        RGB LiS_RGB;
        rayTrace(LiS_RGB, std::move(specularRay));
        LiS.addMult(kS, LiS_RGB);
		}

    // specular transmission
    if (kT.hasColor()) {
        //PDF = 1 / 2 PI
        Vector3D shadingNormalT(shadingNormal);
        float refractiveIndice(intersection.material_->refractiveIndice_);
        if (shadingNormalT.dotProduct(ray.direction_) > 0.0f) {//we are inside the medium
            shadingNormalT *= -1.0f;//N = N*-1;
            refractiveIndice = 1.0f / refractiveIndice;//n = 1 / n;
        }
        refractiveIndice = 1.0f / refractiveIndice;

        const float cosTheta1((shadingNormalT.dotProduct(ray.direction_)) * -1.0f);
        const float cosTheta2(
                1.0f - refractiveIndice * refractiveIndice * (1.0f - cosTheta1 * cosTheta1));
        Ray transmissionRay(cosTheta2 > 0.0f ? // refraction direction
                                  //rayDir = ((ray.d*n) + (N*(n*cost1 - sqrt(cost2)))).norm();
                                  (ray.direction_ * refractiveIndice) +
                                  (shadingNormalT * (refractiveIndice * cosTheta1 -
                                                     (std::sqrt(cosTheta2)))) :
                                  //rayDir = (ray.d + N*(cost1 * 2)).norm();
                                  ray.direction_ + shadingNormalT * (cosTheta1 * 2.0f),
                                  intersection.point_,
                                  rayDepth + 1u);
        RGB LiT_RGB;
        rayTrace(LiT_RGB, std::move(transmissionRay));
        LiT.addMult(kT, LiT_RGB);
		}
	//if (Ld.hasColor()) {LiD.reset();LiS.reset();LiT.reset();}
	Ld /= rayDepth;
	LiD /= rayDepth;
	rgb += Ld;
	rgb += LiD;
	rgb += LiS;
	rgb += LiT;
}

void PathTracer::resetSampling() noexcept {
    this->scene_.resetSampling();
    this->samplerRay_.resetSampling();
    this->samplerLight_.resetSampling();
}
