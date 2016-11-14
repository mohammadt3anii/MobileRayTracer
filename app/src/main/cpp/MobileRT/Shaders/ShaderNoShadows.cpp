//
// Created by Tiago on 16-10-2016.
//

#include "ShaderNoShadows.h"

using namespace MobileRT;

ShaderNoShadows::ShaderNoShadows(RayTracer &rayTracer, const Scene &scene) :
    Shader(rayTracer, scene)
{
}

void ShaderNoShadows::shade(const Ray&, Intersection& intersection, RGB& rgb) const
{
    const RGB& kD (intersection.material_->Kd_);
    rgb.resetRGB ();

    // direct lighting - only for diffuse materials
    if (kD.isZero() == false)
    {
        const unsigned int Nl (scene_.lights.size());
        Vector3D L;

        for (unsigned int l (0) ; l < Nl ; l++)
        {
            const PointLight *ml (scene_.lights[l]);

            L.setVect(ml->position_, intersection.point_);
            L.normalize();
            const float cos_N_L (L.dotProduct(intersection.normal_));
            if (cos_N_L > 0.0f)
            {
                const RGB& radLight (ml->radiance_);
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