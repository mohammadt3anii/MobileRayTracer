//
// Created by puscas on 16-10-2016.
//

#include "ShaderNoShadows.h"
#include "RayTrace.h"

using namespace MobileRT;

ShaderNoShadows::ShaderNoShadows (RayTrace* mRT, Scene mScene) : Shader(mRT, mScene){

}

RGB* ShaderNoShadows::Shade (const Ray& r, const Intersection& isect) {
    RGB* ambient = new RGB (0.1f, 0.1f, 0.1f);
    RGB* rad = new RGB();

    // direct lighting - only for diffuse materials
    if (!isect.material()->Kd.isZero()) {

        int l;
        int Nl = mScene.lights.size();

        for (l=0 ; l < Nl ; l++) {
            Light* ml = mScene.lights[l];

            Vect L(ml->pos - isect.point());
            L.normalize();
            float cos_N_L = L.dot(isect.normal());
            if (cos_N_L > 0.f) {
                rad->R += isect.material()->Kd.R * cos_N_L * ml->rad.R;
                rad->G += isect.material()->Kd.G * cos_N_L * ml->rad.G;
                rad->B += isect.material()->Kd.B * cos_N_L * ml->rad.B;
            }
        }
        // ambient light
        rad->R += isect.material()->Kd.R * ambient->R;
        rad->G += isect.material()->Kd.G * ambient->G;
        rad->B += isect.material()->Kd.B * ambient->B;
    } // end direct + ambient
    return rad;
}