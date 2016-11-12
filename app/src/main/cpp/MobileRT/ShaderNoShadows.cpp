//
// Created by puscas on 16-10-2016.
//

#include "ShaderNoShadows.hpp"

using namespace MobileRT;

ShaderNoShadows::ShaderNoShadows(RayTrace &rayTrace, const Scene &scene) :
    Shader(rayTrace, scene)
{
}

void ShaderNoShadows::Shade(const Ray&, const Intersection &isect, RGB& rgb) const
{
    const RGB& kD = isect.material->Kd;
    rgb.setRGB ();

    // direct lighting - only for diffuse materials
    if (kD.isZero() == false)
    {
        const unsigned int Nl = scene_.lights.size();
        Vect L;

        for (unsigned int l = 0 ; l < Nl ; l++)
        {
            const Light *ml = scene_.lights[l];

            L.setVect(ml->pos, isect.point());
            L.normalize();
            const float cos_N_L = L.dot(isect.normal());
            if (cos_N_L > 0.0f)
            {
                const RGB radLight = ml->rad;
                rgb.R += kD.R * cos_N_L * radLight.R;
                rgb.G += kD.G * cos_N_L * radLight.G;
                rgb.B += kD.B * cos_N_L * radLight.B;
            }
        }
        // ambient light
        rgb.R += kD.R * 0.1f;
        rgb.G += kD.G * 0.1f;
        rgb.B += kD.B * 0.1f;
    } // end direct + ambient
}