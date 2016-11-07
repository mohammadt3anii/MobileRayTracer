//
// Created by puscas on 16-10-2016.
//

#include "ShaderWhitted.h"
#include "Constants.h"

using namespace MobileRT;

ShaderWhitted::ShaderWhitted(RayTrace &rayTrace, Scene &scene) :
        Shader(rayTrace, scene)
{
}

RGB ShaderWhitted::Shade(const Ray &r, const Intersection &isect) const
{
    const float cosRN = r.dir.dot(isect.normal());
    Vect shadingN;

    // the normal always points to outside objects (e.g., spheres)
    // if the cosine between the ray and the normal is less than 0 then
    // the ray intersected the object from the inside and the shading normal
    // should be symmetric to the geometric normal
    if (cosRN < 0.0f)// entering the object
    { 
        shadingN = Vect(isect.normal());
    } else
    {
        // We have to reverse the normal now
        shadingN = isect.normal().symmetric();
    }

    RGB rad;
    // shadowed direct lighting - only for diffuse materials
    if (!isect.material()->getKd().isZero())
    {
        const int Nl = scene_.lights.size();

        for (int l = 0; l < Nl ; l++)//para cada luz
        {
            Light* ml = scene_.lights[l];

            Vect L (ml->getPos() - isect.point());//calcula vetor desde a interseçao até à luz
            const float ml_distance = L.normalize();//distancia do vetor (e normaliza-o)
            const float cos_N_L = L.dot(shadingN);//x*x + y*y + z*z
            if (cos_N_L > 0.0f)
            {
                Point p = isect.point();
                Ray shadowRay (p, L, ml_distance, r.depth+1);//raio de sombra - orig=interseçao, dir=luz
                Intersection Lsect(scene_.shadowTrace(shadowRay));//interseçao do raio de sombra com a primitiva mais proxima
                if (!Lsect.intersected())//se nao ha nenhuma primitiva entre a interseçao e a luz
                {
                    RGB diffuseRad (ml->getRad());//R=1, G=1, B=1
                    diffuseRad.mult(isect.material()->getKd());//cor da luz
                    diffuseRad.mult (cos_N_L);//angulo em relaçao a normal
                    rad.add(diffuseRad);//adiciona a cor da luz
                }
            }
        }
        // ambient light
        rad.R += isect.material()->getKd().R * ambient.R;
        rad.G += isect.material()->getKd().G * ambient.G;
        rad.B += isect.material()->getKd().B * ambient.B;
    } // end direct + ambient
    // specular reflection
    if ((isect.material()->getKs().isZero() == false) && (r.depth < MAX_DEPTH))
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
        RGB specRad(rayTrace_.RayV(specRay));
        specRad.mult(isect.material()->getKs());
        rad.add(specRad);
    }
    return rad;
}