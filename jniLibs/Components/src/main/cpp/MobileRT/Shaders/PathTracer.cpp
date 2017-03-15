//
// Created by puscas on 20-02-2017.
//

#include "PathTracer.h"
#include <cassert>

using namespace MobileRT;

PathTracer::PathTracer(const Scene &scene, Sampler &sampler) :
        Shader(scene),
        sampler_(sampler) {
            /*uniform_dist = std::uniform_real_distribution<float> (0.0f, 1.0f);
            gen = std::mt19937 (rd());*/
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
    const RGB &kD(intersection.material_->Kd_);
    const RGB &kS(intersection.material_->Ks_);
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
        // direct light
        for (unsigned int i(0); i < sizeLights; i++)//for each light
        {
            Light &light(*scene_.lights_[i]);
            //calculates vector starting in intersection to the light
            Point3D lightPosition (light.getPosition());
            Vector3D vectorIntersectCamera(lightPosition, intersection.point_);
            //distance from intersection to the light (and normalize it)
            const float distanceToLight(vectorIntersectCamera.normalize());
            const float cos_N_L(vectorIntersectCamera.dotProduct(shadingNormal));//x*x + y*y + z*z
            if (cos_N_L > 0.0f) {
                //shadow ray - orig=intersection, dir=light
                const Ray shadowRay(intersection.point_, vectorIntersectCamera, distanceToLight,
                                    ray.depth_ + 1);
                //intersection between shadow ray and the closest primitive
                //if there are no primitives between intersection and the light
                if (!scene_.shadowTrace(intersectLight, shadowRay)) {
                    Ld.add(kD, light.radiance_, cos_N_L);//rgb += kD * radLight * cos_N_L;
                }
            }
        }

        if (uniform_dist(gen) > finish_probability)//indirect light
        {
            //Li = PI/N * SOMATORIO i=1 -> i=N [fr (p,Wi <-> Wr) L(p <- Wi)]
            const float r1 (sampler_.getSample(0));
            const float r2 (sampler_.getSample(0));
            //const float r1 (0.0f);
            //const float r2 (1.0f);
            float localX(std::cos(2 * PI * r1) * std::sqrt(1 - r2));
            float localZ(std::sin(2 * PI * r1) * std::sqrt(1 - r2));
            float localY(std::sqrt(r2));
            float localMagnitude(std::sqrt(localX*localX + localY*localY + localZ*localZ));
            localX /= localMagnitude;
            localY /= localMagnitude;
            localZ /= localMagnitude;
            localMagnitude = std::sqrt(localX*localX + localY*localY + localZ*localZ);

            const Vector3D up (0.0f, 1.0f, 0.0f);
            Vector3D u = intersection.normal_.crossProduct(up);
            if (u.x_ == 0.0f && u.y_ == 0.0f && u.z_ == 0.0f) u.x_ = 1.0f;
            const float cosTeta (up.dotProduct(intersection.normal_));
            const float sinTeta (std::sqrt(1.0f - cosTeta*cosTeta));
            const float rotationMatrix [3][3] {
            {cosTeta+u.x_*u.x_*(1.0f-cosTeta), u.x_*u.y_*(1.0f-cosTeta)-u.z_*sinTeta, u.x_*u.z_*(1.0f-cosTeta)+u.y_*sinTeta},
            {u.y_*u.x_*(1.0f-cosTeta)+u.z_*sinTeta, cosTeta+u.y_*u.y_*(1.0f-cosTeta), u.y_*u.z_*(1.0f-cosTeta)-u.x_*sinTeta},
            {u.z_*u.x_*(1.0f-cosTeta)-u.y_*sinTeta, u.z_*u.y_*(1.0f-cosTeta)+u.x_*sinTeta, cosTeta+u.z_*u.z_*(1.0f-cosTeta)}
            };
            float globalX (localX*rotationMatrix[0][0]+localY*rotationMatrix[1][0]+localZ*rotationMatrix[2][0]);
            float globalY (localX*rotationMatrix[0][1]+localY*rotationMatrix[1][1]+localZ*rotationMatrix[2][1]);
            float globalZ (localX*rotationMatrix[0][2]+localY*rotationMatrix[1][2]+localZ*rotationMatrix[2][2]);
            float globalMagnitude (std::sqrt(globalX*globalX + globalY*globalY + globalZ*globalZ));
            globalX /= globalMagnitude;
            globalY /= globalMagnitude;
            globalZ /= globalMagnitude;
            //globalMagnitude = std::sqrt(globalX*globalX + globalY*globalY + globalZ*globalZ);

            //const float normalMagnitude (std::sqrt(intersection.normal_.squareLength()));
            
            Ray newRay (globalX, globalY, globalZ, intersection.point_, ray.depth_ + 1);
            Intersection newIntersection (intersection);

            //if (intersection.normal_.x_ == -1.0f || intersection.normal_.y_ == -1.0f ||
            // intersection.normal_.z_ == -1.0f)
            /*{
                LOG("%s", "");
                LOG("ray depth = %d", ray.depth_);
                LOG("point X=%f, Y=%f, Z=%f", double(intersection.point_.x_),
                    double(intersection.point_.y_), double(intersection.point_.z_));
                LOG("normal X=%f, Y=%f, Z=%f, m=%f", double(intersection.normal_.x_),
                    double(intersection.normal_.y_), double(intersection.normal_.z_),
                    double(normalMagnitude));
                LOG("up X=%f, Y=%f, Z=%f", double(up.x_), double(up.y_), double(up.z_));
                LOG("u X=%f, Y=%f, Z=%f", double(u.x_), double(u.y_), double(u.z_));
                LOG("cosTeta=%f, sinTeta=%f", double(cosTeta), double(sinTeta));
                LOG("local r1=%f, r2=%f", double(r1), double(r2));
                LOG("local X=%f, Y=%f, Z=%f, m=%f", double(localX), double(localY), double(localZ),
                    double(localMagnitude));
                LOG("global X=%f, Y=%f, Z=%f, m=%f",double(globalX), double(globalY),
                    double(globalZ), double(globalMagnitude));
                LOG("Ld=%f,%f,%f", double(Ld.R_), double(Ld.G_), double(Ld.B_));
                //LOG("Li=%f,%f,%f", double(Li.R_), double(Li.G_), double(Li.B_));
                //LOG("RGB=%f,%f,%f", double(rgb.R_), double(rgb.G_), double(rgb.B_));
                //std::cout << uniform_dist(gen) << std::endl;
            }

            assert(!std::isnan(intersection.point_.x_));
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
        }
    }
    // specular reflection
    if (kS.isNotZero()){
        // compute specular reflection
        //reflection ray
        const float RN_dot(2.0f * shadingNormal.dotProduct(ray.symDirection_));
        shadingNormal.mult(RN_dot);
        shadingNormal.subAndNormalize(ray.symDirection_);

        Ray specRay(intersection.point_, shadingNormal, RAY_LENGTH_MAX, ray.depth_ + 1);
        RGB specRad;
        Intersection aux;
        rayTrace(specRad, specRay, aux);
        specRad *= kS;
        Ld.add(specRad);
    }

    Li /= ray_probability;
    Li *= (PI / RAY_DEPTH_MAX);
    rgb.add(Ld);
    rgb.add(Li);

    /*ASSERT(Ld.R_, >=, 0.0f);
    ASSERT(Ld.R_, <=, 1.9f);
    ASSERT(Li.R_, >=, 0.0f);
    ASSERT(Li.R_, <=, 1.9f);*/
}
