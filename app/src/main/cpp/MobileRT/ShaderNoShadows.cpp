//
// Created by Tiago on 16-10-2016.
//

#include "ShaderNoShadows.h"

using namespace MobileRT;

ShaderNoShadows::ShaderNoShadows(RayTrace &rayTrace, const Scene &scene) :
    Shader(rayTrace, scene)
{
}

void ShaderNoShadows::Shade(const Ray&, Intersection& isect, RGB& rgb) const
{
    const RGB& kD (isect.material_->Kd_);
    rgb.resetRGB ();

    // direct lighting - only for diffuse materials
    if (kD.isZero() == false)
    {
        const unsigned int Nl (scene_.lights.size());
        Vect L;

        for (unsigned int l (0) ; l < Nl ; l++)
        {
            const Light *ml (scene_.lights[l]);

            L.setVect(ml->pos_, isect.point_);
            L.normalize();
            const float cos_N_L (L.dot(isect.normal_));
            if (cos_N_L > 0.0f)
            {
                const RGB& radLight (ml->rad_);
                rgb.R_ += kD.R_ * cos_N_L * radLight.R_;
                rgb.G_ += kD.G_ * cos_N_L * radLight.G_;
                rgb.B_ += kD.B_ * cos_N_L * radLight.B_;
            }
        }
        // ambient light
        rgb.R_ += kD.R_ * 0.1f;
        rgb.G_ += kD.G_ * 0.1f;
        rgb.B_ += kD.B_ * 0.1f;
    } // end direct + ambient
}