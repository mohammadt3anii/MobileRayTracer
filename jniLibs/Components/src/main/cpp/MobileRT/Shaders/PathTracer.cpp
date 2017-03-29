//
// Created by puscas on 20-02-2017.
//

#include "PathTracer.h"
//#include <cassert>

using namespace MobileRT;

PathTracer::PathTracer(Scene &scene, Sampler &sampler, const unsigned int samplesLight) :
        Shader(scene, samplesLight),
        sampler_(sampler) {
            /*uniform_dist = std::uniform_real_distribution<float> (0.0f, 1.0f);
            gen = std::mt19937 (rd());*/
}

//pag 28 slides Monte Carlo
void PathTracer::shade(RGB &rgb, Intersection &intersection, Ray &ray) const {
    if (!intersection.material_ || ray.depth_ > RAY_DEPTH_MAX) return;

    static std::uniform_real_distribution<float> uniform_dist(0.0f, 1.0f);
    std::random_device rd;
    static std::mt19937 gen (rd());
    const float finish_probability (0.2f);
    const float ray_probability (1.0f - finish_probability);
    RGB Ld;
    RGB Li;

    const RGB &kD(intersection.material_->Kd_);
    const RGB &kS(intersection.material_->Ks_);
    const RGB &kT(intersection.material_->Kt_);
    const RGB &kE(intersection.material_->Ke_);

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
        const unsigned long sizeLights(scene_.lights_.size());
        Intersection intersectLight;
        const unsigned int samplesLight(static_cast<unsigned int> (std::sqrt(this->samplesLight_)));
        // direct light
        for (unsigned int i(0); i < sizeLights; i++)
        {
            for (unsigned int j(0); j < samplesLight; j++) {
                for (unsigned int k(0); k < samplesLight; k++)//for each light
                {
                    Light &light(*scene_.lights_[i]);
                    //calculates vector starting in intersection to the light
                    const Point3D lightPosition(light.getPosition(j, k));
                    Vector3D vectorIntersectCamera(lightPosition, intersection.point_);
                    //distance from intersection to the light (and normalize it)
                    const float distanceToLight(vectorIntersectCamera.normalize());
                    //x*x + y*y + z*z
                    const float cos_N_L(vectorIntersectCamera.dotProduct(shadingNormal));
                    if (cos_N_L > 0.0f) {
                        //shadow ray - orig=intersection, dir=light
                        const Ray shadowRay(intersection.point_, vectorIntersectCamera,
                                            distanceToLight, ray.depth_ + 1);
                        //intersection between shadow ray and the closest primitive
                        //if there are no primitives between intersection and the light
                        if (!scene_.shadowTrace(intersectLight, shadowRay)) {
                            Ld.add(kD, light.radiance_, cos_N_L);//rgb += kD * radLight * cos_N_L;
                        }
                    }
                }
            }
        }
        Ld /= this->samplesLight_;

        if (ray.depth_ <= RAY_DEPTH_MIN || uniform_dist(gen) > finish_probability)//indirect light
        {
            //Li = PI/N * SOMATORIO i=1 -> i=N [fr (p,Wi <-> Wr) L(p <- Wi)]
            const float r1 (sampler_.getSample(0));
            const float r2 (sampler_.getSample(0));
            float localX(std::cos(2 * PI * r1) * std::sqrt(1 - r2));
            float localZ(std::sin(2 * PI * r1) * std::sqrt(1 - r2));
            float localY(std::sqrt(r2));
            /*float localX(1.0f);
            float localZ(1.0f);
            float localY(1.0f);*/
            float localMagnitude(std::sqrt(localX*localX + localY*localY + localZ*localZ));
            localX /= localMagnitude;
            localY /= localMagnitude;
            localZ /= localMagnitude;
            //localMagnitude = std::sqrt(localX*localX + localY*localY + localZ*localZ);

            const Vector3D up (0.0f, 1.0f, 0.0f);
            Vector3D u = intersection.normal_.crossProduct(up);
            if (u.x_ == 0.0f && u.y_ == 0.0f && u.z_ == 0.0f) u.x_ = 1.0f;
            const float cosTheta(up.dotProduct(intersection.normal_));
            const float sinTheta(std::sqrt(1.0f - cosTheta * cosTheta));
            const float rotationMatrix [3][3] {
                    {cosTheta + u.x_ * u.x_ * (1.0f - cosTheta),
                                                                        u.x_ * u.y_ *
                                                                        (1.0f - cosTheta) -
                                                                        u.z_ * sinTheta,
                            u.x_ * u.z_ * (1.0f - cosTheta) + u.y_ * sinTheta},
                    {u.y_ * u.x_ * (1.0f - cosTheta) + u.z_ * sinTheta, cosTheta + u.y_ * u.y_ *
                                                                                   (1.0f -
                                                                                    cosTheta),
                            u.y_ * u.z_ * (1.0f - cosTheta) - u.x_ * sinTheta},
                    {u.z_ * u.x_ * (1.0f - cosTheta) - u.y_ * sinTheta, u.z_ * u.y_ *
                                                                        (1.0f - cosTheta) +
                                                                        u.x_ * sinTheta,
                            cosTheta + u.z_ * u.z_ * (1.0f - cosTheta)}
            };
            float globalX(localX * rotationMatrix[0][0] + localY * rotationMatrix[1][0] +
                          localZ * rotationMatrix[2][0]);
            float globalY(localX * rotationMatrix[0][1] + localY * rotationMatrix[1][1] +
                          localZ * rotationMatrix[2][1]);
            float globalZ(localX * rotationMatrix[0][2] + localY * rotationMatrix[1][2] +
                          localZ * rotationMatrix[2][2]);
            float globalMagnitude (std::sqrt(globalX*globalX + globalY*globalY + globalZ*globalZ));
            globalX /= globalMagnitude;
            globalY /= globalMagnitude;
            globalZ /= globalMagnitude;
            
            Ray newRay (globalX, globalY, globalZ, intersection.point_, ray.depth_ + 1);
            Intersection newIntersection (intersection);

            /*if (intersection.normal_.x_ == -1.0f || intersection.normal_.y_ == -1.0f ||
             intersection.normal_.z_ == -1.0f)
            {
                LOG("%s", "");
                LOG("ray depth = %d", ray.depth_);
                LOG("point X=%f, Y=%f, Z=%f", double(intersection.point_.x_),
                    double(intersection.point_.y_), double(intersection.point_.z_));
                LOG("normal X=%f, Y=%f, Z=%f, m=%f", double(intersection.normal_.x_),
                    double(intersection.normal_.y_), double(intersection.normal_.z_));
                LOG("up X=%f, Y=%f, Z=%f", double(up.x_), double(up.y_), double(up.z_));
                LOG("u X=%f, Y=%f, Z=%f", double(u.x_), double(u.y_), double(u.z_));
                LOG("cosTheta=%f, sinTheta=%f", double(cosTheta), double(sinTheta));
                LOG("local X=%f, Y=%f, Z=%f, m=%f", double(localX), double(localY), double(localZ),
                    double(localMagnitude));
                LOG("global X=%f, Y=%f, Z=%f, m=%f",double(globalX), double(globalY),
                    double(globalZ), double(globalMagnitude));
                LOG("Ld=%f,%f,%f", double(Ld.R_), double(Ld.G_), double(Ld.B_));
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
            Li.add(newRGB);
            if (newIntersection.material_)
                Li.add(newIntersection.material_->Ke_);
        }
    }

    // specular reflection
    if (kS.isNotZero()){
        // compute specular reflection
        //reflection ray
        Vector3D &shadingNormalR(shadingNormal);
        const float RN_dot(2.0f * shadingNormalR.dotProduct(ray.symDirection_));
        shadingNormalR.mult(RN_dot);
        shadingNormalR.subAndNormalize(ray.symDirection_);//incident + 2 * cosI * normal

        Ray specularRay(intersection.point_, shadingNormalR, RAY_LENGTH_MAX, ray.depth_ + 1);
        RGB specularRad;
        Intersection specularInt;
        rayTrace(specularRad, specularRay, specularInt);
        specularRad *= kS;
        Ld.add(specularRad);
        if (specularInt.material_)
            Ld.add(specularInt.material_->Ke_);
    }

    // specular transmission
    if (kT.isNotZero()) {
        // compute specular transmission
        //transmission ray

        /*Vector3D &shadingNormalT(ray.direction_);
        const float RN_dot(1.0f - 2.0f * shadingNormalT.dotProduct(ray.symDirection_));*/

        //sin2(x) = 1 - cos2(x)
        /*const float sinTheta1 = 1.0f * std::sqrt(1.0f - RN_dot*RN_dot);
        //fresnel equation -> n1 * sin(theta1) = n2 * sin(theta2)
        const float sinTheta2 = sinTheta1 / intersection.refractiveIndice_;
        const float angle (std::asin (sinTheta2));*/
        //LOG("angle = %f", double(angle));

        /*shadingNormalT.mult(RN_dot);
        shadingNormalT.subAndNormalize(ray.symDirection_);

        Ray transmissionRay(intersection.point_, shadingNormalT, RAY_LENGTH_MAX, ray.depth_ + 1);
        RGB transmissionRad;
        Intersection transmissionInt;
        rayTrace(transmissionRad, transmissionRay, transmissionInt);
        transmissionRad *= kT;
        rgb.add(transmissionRad);*/

        //double n = params["refr_index"];
        //double R0 = (1.0 - n) / (1.0 + n);
        //R0 = R0*R0;
        float refractiveIndice(1.0f);
        if (intersection.material_)
            refractiveIndice = intersection.material_->refractiveIndice_;
        //float R0 = (1.0f - refractiveIndice) / (1.0f + refractiveIndice);
        //R0 = R0*R0;
        if (intersection.normal_.dotProduct(ray.direction_) > 0) { // we're inside the medium
            //N = N*-1;
            //n = 1 / n;
            intersection.normal_.mult(-1.0f);
            refractiveIndice = 1.0f / refractiveIndice;
        }
        refractiveIndice = 1.0f / refractiveIndice;

        //double cost1 = (N.dot(ray.d))*-1; // cosine of theta_1
        //double cost2 = 1.0 - n*n*(1.0 - cost1*cost1); // cosine of theta_2
        //double Rprob = R0 + (1.0 - R0) * pow(1.0 - cost1, 5.0); // Schlick-approximation
        const float cost1((intersection.normal_.dotProduct(ray.direction_)) * -1);
        const float cost2(1.0f - refractiveIndice * refractiveIndice * (1.0f - cost1 * cost1));
        //const float Rprob (R0 + (1.0f - R0) * std::pow(1.0f - cost1, 5.0f));
        Ray transmissionRay;
        RGB transmissionRad;
        Intersection transmissionInt;
        if (cost2 > 0 /*&& RND2 > Rprob*/) { // refraction direction
            //ray.d = ((ray.d*n) + (N*(n*cost1 - sqrt(cost2)))).norm();
            transmissionRay.direction_ = ((ray.direction_ * refractiveIndice) +
                                          (intersection.normal_ * (refractiveIndice * cost1 -
                                                                   (std::sqrt(
                                                                           cost2)))));
        } else {// reflection direction
            //ray.d = (ray.d + N*(cost1 * 2)).norm();
            transmissionRay.direction_ = (ray.direction_ + intersection.normal_ * (cost1 * 2));
        }
        transmissionRay.direction_.normalize();
        transmissionRay.depth_ = ray.depth_ + 1;
        transmissionRay.origin_ = intersection.point_;
        transmissionRay.calcSymDirection();
        rayTrace(transmissionRad, transmissionRay, transmissionInt);
        //trace(ray, scene, depth + 1, tmp, params, hal, hal2);
        //clr = clr + tmp * 1.15 * rrFactor;
        transmissionRad *= kT;
        rgb.add(transmissionRad);
    }

    Li /= ray_probability;
    Li *= (PI / RAY_DEPTH_MAX);
    //if (ray.depth_ > 1)
    rgb.add(kE);
    rgb.add(Ld);
    rgb.add(Li);

    /*ASSERT(Ld.R_, >=, 0.0f);
    ASSERT(Ld.R_, <=, 1.9f);
    ASSERT(Li.R_, >=, 0.0f);
    ASSERT(Li.R_, <=, 1.9f);*/
}
