//
// Created by puscas on 20-02-2017.
//

#include "PathTracer.h"

using namespace MobileRT;

PathTracer::PathTracer(Scene &scene, Sampler &samplerRay, Sampler &samplerLight,
                        const unsigned int samplesLight) :
        Shader(scene, samplesLight),
        samplerRay_(samplerRay),
        samplerLight_(samplerLight)
{
            /*uniform_dist = std::uniform_real_distribution<float> (0.0f, 1.0f);
            gen = std::mt19937 (rd());*/
    LOG("sizeLights = %lu, samplesLight = %u", scene_.lights_.size() - 1ul, this->samplesLight_);
}

//pag 28 slides Monte Carlo
void PathTracer::shade(RGB &rgb, Intersection &intersection, Ray &ray) const {
    if (ray.depth_ > RAY_DEPTH_MAX) return;

    static std::uniform_real_distribution<float> uniform_dist(0.0f, 1.0f);
    std::random_device rd;
    static std::mt19937 gen (rd());
    const float finish_probability (0.2f);
    const float ray_probability (1.0f - finish_probability);
    RGB Ld;
    RGB Li;
    RGB Ls;
    RGB Lt;
    const RGB &Le(intersection.material_->Le_);
    const RGB &kD(intersection.material_->Kd_);
    const RGB &kS(intersection.material_->Ks_);
    const RGB &kT(intersection.material_->Kt_);

    // the normal always points to outside objects (e.g., spheres)
    // if the cosine between the ray and the normal is less than 0 then
    // the ray intersected the object from the inside and the shading normal
    // should be symmetric to the geometric normal
    Vector3D &shadingNormal(
            (ray.direction_.dotProduct(intersection.normal_) < 0.0f) ?
            intersection.normal_// entering the object
            // We have to reverse the normal now
            : intersection.getSymNormal());

    // shadowed direct lighting - only for diffuse materials
    //Ld = Ld (p -> Wr)
    if (kD.isNotZero()) {
        const unsigned long sizeLights(scene_.lights_.size() - 1);
        const unsigned int samplesLight(this->samplesLight_);
        Intersection intersectLight;
        //direct light
        for (unsigned int i(0); i < samplesLight; i++)
        {
            //const float qrn (static_cast<float>(rand()) / (RAND_MAX));
            const float qrn (samplerLight_.getSample(0));
            const unsigned int chosenLight (static_cast<unsigned int> ((qrn+0.5f) * sizeLights));
            Light &light(*scene_.lights_[chosenLight]);
            //calculates vector starting in intersection to the light
            const Point3D lightPosition (light.getPosition());
            Vector3D vectorToLight(lightPosition, intersection.point_);
            //distance from intersection to the light (and normalize it)
            const float distanceToLight(vectorToLight.normalize());
            //x*x + y*y + z*z
            const float cosNormalLight(shadingNormal.dotProduct(vectorToLight));
            if (cosNormalLight > 0.0f) {
                //shadow ray - orig=intersection, dir=light
                const Ray shadowRay(intersection.point_, vectorToLight,
                                    distanceToLight, ray.depth_ + 1);
                //intersection between shadow ray and the closest primitive
                //if there are no primitives between intersection and the light
                if (!scene_.shadowTrace(intersectLight, shadowRay)) {
                    //Ld += kD * radLight * cosNormalLight;
                    Ld.add(light.radiance_, cosNormalLight);
                }
            }
        }

        //indirect light
        if (ray.depth_ <= RAY_DEPTH_MIN || uniform_dist(gen) > finish_probability)
        {
            //Li = PI/N * SOMATORIO i=1 -> i=N [fr (p,Wi <-> Wr) L(p <- Wi)]
            //const float r1 (samplerRay_.getSample(0));
            //const float r2 (samplerRay_.getSample(0));
            const float r1 (static_cast<float>(rand()) / (RAND_MAX));
            const float r2 (static_cast<float>(rand()) / (RAND_MAX));
            float localX(std::cos(2 * PI * r1) * std::sqrt(1 - r2));
            float localZ(std::sin(2 * PI * r1) * std::sqrt(1 - r2));
            float localY(std::sqrt(r2));
            /*float localX(0.0f);
            float localY(1.0f);
            float localZ(0.0f);*/
            float localMagnitude(std::sqrt(localX*localX + localY*localY + localZ*localZ));
            localX /= localMagnitude;
            localY /= localMagnitude;
            localZ /= localMagnitude;
            //localMagnitude = std::sqrt(localX*localX + localY*localY + localZ*localZ);

            const Vector3D up (0.0f, 1.0f, 0.0f);
            Vector3D u = intersection.normal_.crossProduct(up);
            if (u.x_ == 0.0f && u.y_ == 0.0f && u.z_ == 0.0f) u.x_ = 1.0f;
            const float cosTheta (up.dotProduct(intersection.normal_));
            float sinThetaSq (1.0f - cosTheta*cosTheta);
            if (sinThetaSq < 0.0f) sinThetaSq *= -1.0f;
            const float sinTheta (std::sqrt(sinThetaSq));
            const float rotationMatrix [3][3]
            {
                {
                    cosTheta+u.x_*u.x_*(1.0f-cosTheta),
                    u.x_*u.y_*(1.0f-cosTheta)-u.z_*sinTheta,
                    u.x_*u.z_*(1.0f-cosTheta)+u.y_*sinTheta
                },

                {
                    u.y_*u.x_*(1.0f-cosTheta)+u.z_*sinTheta,
                    cosTheta+u.y_*u.y_*(1.0f-cosTheta),
                    u.y_*u.z_*(1.0f-cosTheta)-u.x_*sinTheta
                },

                {
                    u.z_*u.x_*(1.0f-cosTheta)-u.y_*sinTheta,
                    u.z_*u.y_*(1.0f-cosTheta)+u.x_*sinTheta,
                    cosTheta+u.z_*u.z_*(1.0f-cosTheta)
                }
            };
            float globalX ( localX*rotationMatrix[0][0]+
                            localY*rotationMatrix[1][0]+
                            localZ*rotationMatrix[2][0]);

            float globalY ( localX*rotationMatrix[0][1]+
                            localY*rotationMatrix[1][1]+
                            localZ*rotationMatrix[2][1]);

            float globalZ ( localX*rotationMatrix[0][2]+
                            localY*rotationMatrix[1][2]+
                            localZ*rotationMatrix[2][2]);

            float globalMagnitude (std::sqrt(
                globalX*globalX + globalY*globalY + globalZ*globalZ));

            globalX /= globalMagnitude;
            globalY /= globalMagnitude;
            globalZ /= globalMagnitude;
            
            Ray newRay (globalX, globalY, globalZ, intersection.point_, ray.depth_ + 1);
            Intersection newIntersection (intersection);

            /*if (intersection.normal_.x_ <= 0.0f && intersection.normal_.y_ >= 1.0f
                && intersection.normal_.z_ <= 0.0f)
            {
                LOG("%s", "");
                LOG("ray depth = %d", ray.depth_);
                LOG("origin X=%.7f, Y=%.7f, Z=%.7f", double(intersection.origin_.x_),
                    double(intersection.origin_.y_), double(intersection.origin_.z_));
                LOG("origin2 X=%.7f, Y=%.7f, Z=%.7f", double(ray.origin_.x_),
                    double(ray.origin_.y_), double(ray.origin_.z_));
                LOG("point X=%.7f, Y=%.7f, Z=%.7f", double(intersection.point_.x_),
                    double(intersection.point_.y_), double(intersection.point_.z_));
                LOG("normal X=%.7f, Y=%.7f, Z=%.7f", double(intersection.normal_.x_),
                    double(intersection.normal_.y_), double(intersection.normal_.z_));
                LOG("up X=%.7f, Y=%.7f, Z=%.7f", double(up.x_), double(up.y_), double(up.z_));
                LOG("u X=%.7f, Y=%.7f, Z=%.7f", double(u.x_), double(u.y_), double(u.z_));
                LOG("cosTheta=%.7f, sinTheta=%.7f sinThetaSq=%.7f",
                    double(cosTheta), double(sinTheta), double(sinThetaSq));
                LOG("local X=%.7f, Y=%.7f, Z=%.7f, m=%.7f", double(localX), double(localY),
                    double(localZ), double(localMagnitude));
                LOG("global X=%.7f, Y=%.7f, Z=%.7f, m=%.7f",double(globalX), double(globalY),
                    double(globalZ), double(globalMagnitude));
                LOG("Ld=%.7f,%.7f,%.7f", double(Ld.R_), double(Ld.G_), double(Ld.B_));
                raise(SIGTRAP);
            }*/

            /*assert(!std::isnan(intersection.point_.x_));
            assert(!std::isnan(intersection.point_.y_));
            assert(!std::isnan(intersection.point_.z_));
            assert(!std::isnan(intersection.normal_.x_));
            assert(!std::isnan(intersection.normal_.y_));
            assert(!std::isnan(intersection.normal_.z_));
            assert(!std::isnan(intersection.length_));
            assert(!std::isnan(intersection.symNormal_.x_));
            assert(!std::isnan(intersection.symNormal_.y_));
            assert(!std::isnan(intersection.symNormal_.z_));
            assert(!std::isnan(ray.depth_));
            assert(!std::isnan(ray.direction_.x_));
            assert(!std::isnan(ray.direction_.y_));
            assert(!std::isnan(ray.direction_.z_));
            assert(!std::isnan(ray.maxDistance_));
            assert(!std::isnan(ray.origin_.x_));
            assert(!std::isnan(ray.origin_.y_));
            assert(!std::isnan(ray.origin_.z_));
            assert(!std::isnan(ray.symDirection_.x_));
            assert(!std::isnan(ray.symDirection_.y_));
            assert(!std::isnan(ray.symDirection_.z_));
            ASSERT(ray.depth_, >=, 1);
            ASSERT(ray.depth_, <=, RAY_DEPTH_MAX);
            ASSERT(r1, >=, 0.0f);
            ASSERT(r1, <=, 1.0f);
            ASSERT(r2, >=, 0.0f);
            ASSERT(r2, <=, 1.0f);
            ASSERT(globalMagnitude, <=, 1.0f + 0.00001f);
            ASSERT(globalMagnitude, >=, 1.0f - 0.00001f);
            ASSERT(localMagnitude, <=, 1.0f + 0.00001f);
            ASSERT(localMagnitude, >=, 1.0f - 0.00001f);
            ASSERT(normalMagnitude, <=, 1.0f + 0.00001f);
            ASSERT(normalMagnitude, >=,  1.0f - 0.00001f);
            ASSERT(finish_probability, >=, 0.0f);
            ASSERT(finish_probability, <=, 1.0f);*/

            RGB newRGB;
            rayTrace(newRGB, newRay, newIntersection);
            Vector3D vectorToNewPoint(newIntersection.point_, intersection.point_);
            vectorToNewPoint.normalize();
            const float cosNormalNewPoint(shadingNormal.dotProduct(vectorToNewPoint));
            //Li += newRGB * cosNormalNewPoint
            Li.add(newRGB, 2.0f * cosNormalNewPoint);
        }
    }

    // specular reflection
    if (kS.isNotZero()){
        //reflectionDir = rayDirection - (2 * rayDirection . normal) * normal
        /*const float RN_dot(2.0f * shadingNormal.dotProduct(ray.direction_));
        Vector3D auxDir(shadingNormal, RN_dot);
        //auxDir.mult(RN_dot);
        Vector3D reflectionDir (ray.direction_);
        reflectionDir.subAndNormalize(auxDir);*/

        //reflectionDir = (2 * symRayDirection . normal) * normal - symRayDirection
        const float RN_dot(2.0f * shadingNormal.dotProduct(ray.symDirection_));
        Vector3D reflectionDir (shadingNormal, RN_dot);
        //reflectionDir.mult(RN_dot);
        reflectionDir.subAndNormalize(ray.symDirection_);

        Ray specularRay(intersection.point_, reflectionDir, RAY_LENGTH_MAX, ray.depth_ + 1);
        Intersection specularInt;
        rayTrace(Ls, specularRay, specularInt);
    }

    // specular transmission
    if (kT.isNotZero()){
        float refractiveIndice(intersection.material_->refractiveIndice_);
        if (shadingNormal.dotProduct(ray.direction_) > 0.0f) {
            //we are inside the medium
            shadingNormal.mult(-1.0f);
            refractiveIndice = 1.0f / refractiveIndice;
        }
        refractiveIndice = 1.0f / refractiveIndice;

        const float cosTheta1 (-shadingNormal.dotProduct(ray.direction_));
        const float cosTheta2 (1.0f - refractiveIndice*refractiveIndice*(1.0f-cosTheta1*cosTheta1));
        Ray transmissionRay;
        if (cosTheta2 > 0.0f) {
            //refraction direction
            //ray.d = ((ray.d*n) + (N*(n*cost1 - sqrt(cost2)))).norm();
            transmissionRay.direction_ = ((ray.direction_*refractiveIndice) +
                                          (shadingNormal*(refractiveIndice*cosTheta1 -
                                                         (std::sqrt(cosTheta2)))));
        } else {
            //reflection direction
            //ray.d = (ray.d + N*(cost1 * 2)).norm();
            transmissionRay.direction_ = (ray.direction_ + shadingNormal*(cosTheta1 * 2.0f));
        }
        transmissionRay.direction_.normalize();
        transmissionRay.depth_ = ray.depth_ + 1;
        transmissionRay.origin_ = intersection.point_;
        transmissionRay.calcSymDirection();
        Intersection transmissionInt;
        rayTrace(Lt, transmissionRay, transmissionInt);
    }

    Ld /= this->samplesLight_;
    Ld *= kD;

    Li /= ray_probability;
    const float value (PI / RAY_DEPTH_MAX);
    //LOG("v = %f", double(value));
    Li *= value;//(PI / RAY_DEPTH_MIN);
    Li *= kD;
    //if (ray.depth_ == 1)
    //LOG("Li = %f, %f, %f", double(Li.R_), double(Li.G_), double(Li.B_));

    Ls *= kS;
    Lt *= kT;

    //if (ray.depth_ > 1)
    rgb.add(Ld);
    rgb.add(Li);

    rgb.add(Le);
    rgb.add(Ls);
    rgb.add(Lt);
}

void PathTracer::resetSampling(void) {
    this->scene_.resetSampling();
    this->samplerRay_.resetSampling();
    this->samplerLight_.resetSampling();
}
