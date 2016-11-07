//
// Created by puscas on 16-10-2016.
//

#include "ShaderNoShadows.h"

using namespace MobileRT;

ShaderNoShadows::ShaderNoShadows(RayTrace &rayTrace, Scene &scene) :
    Shader(rayTrace, scene)
{
}

RGB ShaderNoShadows::Shade(const Ray&, const Intersection &isect) const
{
    RGB rad;

    // direct lighting - only for diffuse materials
    if (!isect.material()->getKd().isZero())
    {
        int Nl = scene_.lights.size();

        for (int l=0 ; l < Nl ; l++)
        {
            Light *ml = scene_.lights[l];

            Vect L(ml->getPos() - isect.point());
            L.normalize();
            float cos_N_L = L.dot(isect.normal());
            if (cos_N_L > 0.0f)
            {
                rad.R += isect.material()->getKd().R * cos_N_L * ml->getRad().R;
                rad.G += isect.material()->getKd().G * cos_N_L * ml->getRad().G;
                rad.B += isect.material()->getKd().B * cos_N_L * ml->getRad().B;
            }
        }
        // ambient light
        RGB ambient(0.1f, 0.1f, 0.1f);
        const RGB& v = isect.material()->getKd();
        rad.R += v.R * ambient.R;
        rad.G += v.G * ambient.G;
        rad.B += v.B * ambient.B;
    } // end direct + ambient
    return rad;
}