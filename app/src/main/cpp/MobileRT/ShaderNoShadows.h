//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADERNOSHADOWS_H
#define MOBILERAYTRACER_SHADERNOSHADOWS_H

#include "Shader.h"

namespace MobileRT{
    class ShaderNoShadows : public Shader {
    private:
    public:
        ShaderNoShadows (RayTrace* mRT, Scene mScene);
        RGB* Shade (Ray r, Intersection isect);
    };
}

#endif //MOBILERAYTRACER_SHADERNOSHADOWS_H

/*
package com.example.lps.mobilert;

public class ShaderNoShadows extends Shader {

ShaderNoShadows (RayTrace mRT, Scene mScene) {
super (mRT, mScene);
}

public RGB Shade (Ray r, Intersection isect) {
    final RGB ambient = new RGB (0.1f, 0.1f, 0.1f);
    RGB rad = new RGB();

    // direct lighting - only for diffuse materials
    if (!isect.m.Kd.isZero()) {
        myVect L = new myVect();
        int l;
        final int Nl = mScene.lights.size();

        for (l=0 ; l < Nl ; l++) {
            Light ml = mScene.lights.get(l);

            L.x = ml.pos.x - isect.p.x;
            L.y = ml.pos.y - isect.p.y;
            L.z = ml.pos.z - isect.p.z;
            L.normalize();
            float cos_N_L = L.dot(isect.N);
            if (cos_N_L > 0.f) {
                rad.R += isect.m.Kd.R * cos_N_L * ml.rad.R;
                rad.G += isect.m.Kd.G * cos_N_L * ml.rad.G;
                rad.B += isect.m.Kd.B * cos_N_L * ml.rad.B;
            }
        }
        // ambient light
        rad.R += isect.m.Kd.R * ambient.R;
        rad.G += isect.m.Kd.G * ambient.G;
        rad.B += isect.m.Kd.B * ambient.B;
    } // end direct + ambient
    return rad;
}

}
*/