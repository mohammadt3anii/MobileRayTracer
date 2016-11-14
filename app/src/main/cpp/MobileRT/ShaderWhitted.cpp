//
// Created by Tiago on 16-10-2016.
//

#include "ShaderWhitted.h"
#include "Constants.h"

using namespace MobileRT;

ShaderWhitted::ShaderWhitted(RayTrace& rayTrace, const Scene& scene) :
        Shader(rayTrace, scene)
{
}

void ShaderWhitted::Shade(const Ray& ray, Intersection& isect, RGB& rgb) const
{
    // the normal always points to outside objects (e.g., spheres)
    // if the cosine between the ray and the normal is less than 0 then
    // the ray intersected the object from the inside and the shading normal
    // should be symmetric to the geometric normal
    Vect& shadingN ((ray.direction_.dot(isect.normal_) < 0.0f)?
                          isect.normal_// entering the object
                        : isect.getSymNormal());// We have to reverse the normal now

    rgb.resetRGB();
    // shadowed direct lighting - only for diffuse materials
    if (isect.material_->Kd_.isZero() == false)
    {
        const unsigned int Nl (scene_.lights.size());
        Intersection Lsect;
        Vect L;

        for (unsigned int l (0); l < Nl ; l++)//para cada luz
        {
            const Light* ml (scene_.lights[l]);

            L.setVect(ml->pos_, isect.point_);//calcula vetor desde a interseçao até à luz
            const float ml_distance (L.normalize());//distancia do vetor (e normaliza-o)
            const float cos_N_L (L.dot(shadingN));//x*x + y*y + z*z
            if (cos_N_L > 0.0f)
            {
                const Ray shadowRay(isect.point_, L, ml_distance,
                              ray.depth_ + 1);//raio de sombra - orig=interseçao, dir=luz
                //Lsect = ();//interseçao do raio de sombra com a primitiva mais proxima
                if (scene_.shadowTrace(shadowRay, Lsect) == false)//se nao ha nenhuma primitiva entre a interseçao e a luz
                {
                    RGB diffuseRad(ml->rad_);//R=1, G=1, B=1
                    diffuseRad.mult(isect.material_->Kd_);//cor da luz
                    diffuseRad.mult (cos_N_L);//angulo em relaçao a normal
                    rgb.add(diffuseRad);//adiciona a cor da luz
                }
            }
        }
        // ambient light
        rgb.R_ += isect.material_->Kd_.R_ * 0.1f;
        rgb.G_ += isect.material_->Kd_.G_ * 0.1f;
        rgb.B_ += isect.material_->Kd_.B_ * 0.1f;
    } // end direct + ambient
    // specular reflection
    if ((isect.material_->Ks_.isZero() == false) && (ray.depth_ < this->MAX_DEPTH))
    {
        // compute specular reflection
        //raio de reflexao
        const float RN_dot (2.0f * shadingN.dot(ray.symDirection_));
        shadingN.mult (RN_dot);
        shadingN.sub (ray.symDirection_);
        shadingN.normalize();

        Ray specRay(isect.point_, shadingN, MAX_LENGTH, ray.depth_ + 1);
        RGB specRad;
        Intersection isec;
        this->rayTrace_.RayV(specRay, specRad, isec);
        specRad.mult(isect.material_->Ks_);
        rgb.add(specRad);
    }
}