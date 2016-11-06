//
// Created by puscas on 16-10-2016.
//

#include "ShaderNoShadows.h"

using namespace MobileRT;

ShaderNoShadows::ShaderNoShadows(RayTrace &rayTrace, Scene &scene) :
        Shader(rayTrace, scene) {
}

RGB ShaderNoShadows::Shade(const Ray &r, const Intersection &isect) {

    RGB rad;

    // direct lighting - only for diffuse materials
    if (!isect.material()->Kd.isZero()) {
        int l;
        int Nl = scene_.lights.size();

        for (l=0 ; l < Nl ; l++) {
            Light *ml = scene_.lights[l];

            Vect L(ml->pos - isect.point());
            L.normalize();
            float cos_N_L = L.dot(isect.normal());
            if (cos_N_L > 0.0f) {
                rad.R += isect.material()->Kd.R * cos_N_L * ml->rad.R;
                rad.G += isect.material()->Kd.G * cos_N_L * ml->rad.G;
                rad.B += isect.material()->Kd.B * cos_N_L * ml->rad.B;
            }
        }
        // ambient light
        RGB ambient(0.1f, 0.1f, 0.1f);
        const RGB& v = isect.material()->Kd;
        rad.R += v.R * ambient.R;
        rad.G += v.G * ambient.G;
        rad.B += v.B * ambient.B;
    } // end direct + ambient
    return rad;
}