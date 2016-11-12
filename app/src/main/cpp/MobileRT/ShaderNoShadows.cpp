//
// Created by puscas on 16-10-2016.
//

#include "ShaderNoShadows.hpp"

using namespace MobileRT;

ShaderNoShadows::ShaderNoShadows(RayTrace &rayTrace, const Scene &scene) :
    Shader(rayTrace, scene)
{
}

RGB ShaderNoShadows::Shade(const Ray&, const Intersection &isect) const
{
    RGB rad;
    const RGB& kD = isect.material->Kd;

    // direct lighting - only for diffuse materials
    if (kD.isZero() == false)
    {
        const unsigned int Nl = scene_.lights.size();

        for (unsigned int l = 0 ; l < Nl ; l++)
        {
            const Light *ml = scene_.lights[l];

            Vect L(ml->pos - isect.point());
            L.normalize();
            const float cos_N_L = L.dot(isect.normal());
            if (cos_N_L > 0.0f)
            {
                const RGB radLight = ml->rad;
                rad.R += kD.R * cos_N_L * radLight.R;
                rad.G += kD.G * cos_N_L * radLight.G;
                rad.B += kD.B * cos_N_L * radLight.B;
            }
        }
        // ambient light
        rad.R += kD.R * 0.1f;
        rad.G += kD.G * 0.1f;
        rad.B += kD.B * 0.1f;
    } // end direct + ambient
    return rad;
}