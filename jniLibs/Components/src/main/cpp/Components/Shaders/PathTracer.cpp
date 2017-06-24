//
// Created by puscas on 20-02-2017.
//

#include "PathTracer.h"

using namespace Components;

PathTracer::PathTracer(Scene &scene, Sampler &samplerRay, Sampler &samplerLight,
                       const unsigned int samplesLight) :
        Shader(scene, samplesLight),
        samplerRay_(samplerRay),
        samplerLight_(samplerLight) {
    LOG("sizeLights = %lu", long(scene_.lights_.size()));
    LOG("samplesLight = %u", this->samplesLight_);
}

//pag 28 slides Monte Carlo
void PathTracer::shade(RGB &rgb, const Intersection &intersection, const Ray &ray) const {
    const unsigned int rayDepth(ray.depth_);
    if (rayDepth > RAY_DEPTH_MAX) return;

    const RGB &Le(intersection.material_->Le_);
    if (Le.isNotZero())//stop if it intersects a light source
    {
        rgb.add(Le * scene_.lights_.size() * RAY_DEPTH_MAX);
        return;
    }

    const RGB &kD(intersection.material_->Kd_);
    const RGB &kS(intersection.material_->Ks_);
    const RGB &kT(intersection.material_->Kt_);

    static std::uniform_real_distribution<float> uniform_dist(0.0f, 1.0f);
    std::random_device rd;
    static std::mt19937 gen(rd());
    const float finish_probability(0.5f);
    const float continue_probability(1.0f - finish_probability);

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
    if (kD.isNotZero()) {
        const unsigned long sizeLights(scene_.lights_.size() - 1ul);
        const unsigned int samplesLight(this->samplesLight_);
        Intersection intersectLight;
        //direct light
        for (unsigned int i(0u); i < samplesLight; i++) {
            const float qrn(samplerLight_.getSample(0u));
            //PDF = 1 / sizeLights
            const unsigned int chosenLight(
                    static_cast<unsigned int> ((qrn + 0.5f) * sizeLights));
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
                const Ray shadowRay(vectorToLight, intersection.point_, rayDepth);
                //intersection between shadow ray and the closest primitive
                //if there are no primitives between intersection and the light
                intersectLight.length_ = distanceToLight;
                if (!scene_.shadowTrace(intersectLight, shadowRay)) {
                    //Ld += kD * radLight * cosNormalLight * sizeLights / samplesLight
                    rgb.addMult(light.radiance_.Le_, cosNormalLight);
                }
            }
            rgb *= kD;
            rgb *= sizeLights + 1ul;
            rgb /= this->samplesLight_;
        }

        //indirect light
        if (rayDepth <= RAY_DEPTH_MIN || uniform_dist(gen) > finish_probability) {
            //Li = PI/N * SOMATORIO i=1 -> i=N [fr (p,Wi <-> Wr) L(p <- Wi)]
            //estimator = <F^N>=1/N * ∑(i=0)(N−1) f(Xi) / pdf(Xi)

            /*float localX(1.0f);
            float localY(2.0f);
            float localZ(-3.0f);*/
            const float r1(samplerRay_.getSample(0u));
            const float r2(samplerRay_.getSample(0u));
            samplerRay_.getSample(0u);
            float localX(std::cos(2.0f * PI * r1) * std::sqrt(1.0f - r2));
            float localZ(std::sin(2.0f * PI * r1) * std::sqrt(1.0f - r2));
            float localY(std::sqrt(r2));
            const float localMagnitude(
                    std::sqrt(localX * localX + localY * localY + localZ * localZ));
            localX /= localMagnitude;
            localY /= localMagnitude;
            localZ /= localMagnitude;

            const Vector3D up(0.0f, 1.0f, 0.0f);
            Vector3D u(intersection.normal_.crossProduct(up));
            if (u.x_ == 0.0f && u.y_ == 0.0f && u.z_ == 0.0f) u.x_ = 1.0f;
            const float cosTheta(up.dotProduct(intersection.normal_));
            //const float cosTheta (1.0f);
            float sinThetaSq(1.0f - cosTheta * cosTheta);
            if (sinThetaSq < 0.0f) sinThetaSq *= -1.0f;
            const float sinTheta(std::sqrt(sinThetaSq));
            const float rotationMatrix[3][3]
                    {
                            {
                                    cosTheta + u.x_ * u.x_ * (1.0f - cosTheta),
                                    u.x_ * u.y_ * (1.0f - cosTheta) - u.z_ * sinTheta,
                                    u.x_ * u.z_ * (1.0f - cosTheta) + u.y_ * sinTheta
                            },

                            {
                                    u.y_ * u.x_ * (1.0f - cosTheta) + u.z_ * sinTheta,
                                    cosTheta + u.y_ * u.y_ * (1.0f - cosTheta),
                                    u.y_ * u.z_ * (1.0f - cosTheta) - u.x_ * sinTheta
                            },

                            {
                                    u.z_ * u.x_ * (1.0f - cosTheta) - u.y_ * sinTheta,
                                    u.z_ * u.y_ * (1.0f - cosTheta) + u.x_ * sinTheta,
                                    cosTheta + u.z_ * u.z_ * (1.0f - cosTheta)
                            }
                    };

            float globalX(localX * rotationMatrix[0][0] +
                          localY * rotationMatrix[1][0] +
                          localZ * rotationMatrix[2][0]);

            float globalY(localX * rotationMatrix[0][1] +
                          localY * rotationMatrix[1][1] +
                          localZ * rotationMatrix[2][1]);

            float globalZ(localX * rotationMatrix[0][2] +
                          localY * rotationMatrix[1][2] +
                          localZ * rotationMatrix[2][2]);

            float globalMagnitude(std::sqrt(
                    globalX * globalX + globalY * globalY + globalZ * globalZ));
            globalX /= globalMagnitude;
            globalY /= globalMagnitude;
            globalZ /= globalMagnitude;

            //secundaryRay -> origin =  intersection.point_, direction = global, depth = rayDepth+1
            const Ray normalizedSecundaryRay(globalX, globalY, globalZ, intersection.point_,
                                       rayDepth + 1u);
            //secundaryIntersection = intersection
            Intersection secundaryIntersection;

            RGB LiD_RGB;
            rayTrace(LiD_RGB, normalizedSecundaryRay, secundaryIntersection);

            //PDF = cos(theta) / PI
            //cos (theta) = cos(dir, normal)
            //LiD += kD * LiD_RGB * cos (dir, normal) / (PDF * continue_probability)
            //LiD += kD * LiD_RGB * PI / continue_probability

            rgb.addMult(kD, LiD_RGB, PI);
            //LiD.addMult(kD, LiD_RGB);
            if (rayDepth > RAY_DEPTH_MIN) rgb /= continue_probability;
            //LiD /= (rayDepth + 1u);
            //if it intersects a light source then LiD = 0
            if (secundaryIntersection.material_ != nullptr &&
                secundaryIntersection.material_->Le_.isNotZero())
                rgb.reset();
        }
    }

    // specular reflection
    if (kS.isNotZero()) {
        //PDF = 1 / 2 PI

        //reflectionDir = rayDirection - (2 * rayDirection . normal) * normal
        const Vector3D reflectionDir(
                ray.direction_, shadingNormal, 2.0f * shadingNormal.dotProduct(ray.direction_));

        const Ray specularRay(reflectionDir, intersection.point_, rayDepth + 1u);
        RGB LiS_RGB;
        Intersection specularInt;
        rayTrace(LiS_RGB, specularRay, specularInt);
        rgb.addMult(kS, LiS_RGB);
    }

    // specular transmission
    if (kT.isNotZero()) {
        //PDF = 1 / 2 PI

        Vector3D shadingNormalT(shadingNormal);
        float refractiveIndice(intersection.material_->refractiveIndice_);
        if (shadingNormalT.dotProduct(ray.direction_) > 0.0f) {//we are inside the medium
            shadingNormalT.mult(-1.0f);//N = N*-1;
            refractiveIndice = 1.0f / refractiveIndice;//n = 1 / n;
        }
        refractiveIndice = 1.0f / refractiveIndice;

        const float cosTheta1((shadingNormalT.dotProduct(ray.direction_)) * -1.0f);
        const float cosTheta2(
                1.0f - refractiveIndice * refractiveIndice * (1.0f - cosTheta1 * cosTheta1));
        const Ray transmissionRay(cosTheta2 > 0.0f ? // refraction direction
                                  //rayDir = ((ray.d*n) + (N*(n*cost1 - sqrt(cost2)))).norm();
                                  (ray.direction_ * refractiveIndice) +
                                  (shadingNormalT * (refractiveIndice * cosTheta1 -
                                                     (std::sqrt(cosTheta2)))) :
                                  //rayDir = (ray.d + N*(cost1 * 2)).norm();
                                  ray.direction_ + shadingNormalT * (cosTheta1 * 2.0f),
                                  intersection.point_,
                                  rayDepth + 1u);
        RGB LiT_RGB;
        Intersection transmissionInt;
        rayTrace(LiT_RGB, transmissionRay, transmissionInt);
        rgb.addMult(kT, LiT_RGB);
    }
}

void PathTracer::resetSampling(void) {
    this->scene_.resetSampling();
    this->samplerRay_.resetSampling();
    this->samplerLight_.resetSampling();
}
