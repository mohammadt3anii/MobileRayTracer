//
// Created by puscas on 16-10-2016.
//

#include "ShaderWhitted.hpp"
#include "Constants.hpp"

using namespace MobileRT;

ShaderWhitted::ShaderWhitted(RayTrace& rayTrace, const Scene& scene) :
        Shader(rayTrace, scene)
{
}

RGB ShaderWhitted::Shade(const Ray& r, const Intersection& isect) const
{
    const float cosRN = r.dir.dot(isect.normal());
    // the normal always points to outside objects (e.g., spheres)
    // if the cosine between the ray and the normal is less than 0 then
    // the ray intersected the object from the inside and the shading normal
    // should be symmetric to the geometric normal
    const Vect shadingN = (cosRN < 0.0f)?
        Vect(isect.normal())// entering the object
        : isect.normal().symmetric();// We have to reverse the normal now

    RGB rad;
    // shadowed direct lighting - only for diffuse materials
    if (isect.material->Kd.isZero() == false)
    {
        const unsigned int Nl = scene_.lights.size();
        const Intersection* Lsect;

        for (unsigned int l = 0; l < Nl ; l++)//para cada luz
        {
            const Light* ml = scene_.lights[l];

            Vect L (ml->pos - isect.point());//calcula vetor desde a interseçao até à luz
            const float ml_distance = L.normalize();//distancia do vetor (e normaliza-o)
            const float cos_N_L = L.dot(shadingN);//x*x + y*y + z*z
            if (cos_N_L > 0.0f)
            {
                Point p = isect.point();
                Ray shadowRay (p, L, ml_distance, r.depth+1);//raio de sombra - orig=interseçao, dir=luz
                Lsect = (scene_.shadowTrace(shadowRay));//interseçao do raio de sombra com a primitiva mais proxima
                if (Lsect == nullptr)//se nao ha nenhuma primitiva entre a interseçao e a luz
                {
                    RGB diffuseRad (ml->rad);//R=1, G=1, B=1
                    diffuseRad.mult(isect.material->Kd);//cor da luz
                    diffuseRad.mult (cos_N_L);//angulo em relaçao a normal
                    rad.add(diffuseRad);//adiciona a cor da luz
                }
            }
        }
        // ambient light
        rad.R += isect.material->Kd.R * 0.1f;
        rad.G += isect.material->Kd.G * 0.1f;
        rad.B += isect.material->Kd.B * 0.1f;
    } // end direct + ambient
    // specular reflection
    if ((isect.material->Ks.isZero() == false) && (r.depth < this->MAX_DEPTH))
    {
        // compute specular reflection
        const Vect sym_vRay = r.dir.symmetric();//raio de reflexao
        const float RN_dot = 2.0f * shadingN.dot(sym_vRay);
        Vect specDir = shadingN;
        specDir.mult (RN_dot);
        specDir.sub (sym_vRay);
        specDir.normalize();

        const Point p = isect.point();
        const Ray specRay(p, specDir, MAX_LENGTH, r.depth+1);
        RGB specRad(this->rayTrace_.RayV(specRay));
        specRad.mult(isect.material->Ks);
        rad.add(specRad);
    }
    delete &isect;
    return rad;
}