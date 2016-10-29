//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADERWHITTED_H
#define MOBILERAYTRACER_SHADERWHITTED_H

#include "Shader.h"

namespace MobileRT {
    class ShaderWhitted : public Shader {
    private:

    public:
        int MAX_DEPTH;
        RGB ambient;
        ShaderWhitted (RayTrace* mRT, Scene mScene);
        virtual RGB* Shade (const Ray& r, const Intersection& isect);
    };
}

#endif //MOBILERAYTRACER_SHADERWHITTED_H

/*
package com.example.lps.mobilert;

public class ShaderWhitted extends Shader {
final int MAX_DEPTH=5;
final RGB ambient = new RGB (0.1f, 0.1f, 0.1f);

ShaderWhitted (RayTrace mRT, Scene mScene) {
super (mRT, mScene);
}

public RGB Shade (Ray r, Intersection isect) {
    RGB rad = new RGB();
    Vect shadingN;
    final float cosRN = r.dir.dot(isect.N);

    // the normal always points to outside objects (e.g., spheres)
    // if the cosine between the ray and the normal is less than 0 then
    // the ray intersected the object from the inside and the shading normal
    // should be symmetric to the geometric normal
    if (cosRN<0.f) { // entering the object
        shadingN = new Vect(isect.N);
    } else {
        // We have to reverse the normal now
        shadingN = isect.N.symmetric();
    }


    // shadowed direct lighting - only for diffuse materials
    if (!isect.m.Kd.isZero()) {
        Vect L = new Vect();
        int l;
        final int Nl = mScene.lights.size();

        for (l=0 ; l < Nl ; l++) {
            Light ml = mScene.lights.get(l);
            float ml_distance;

            L.x = ml.pos.x - isect.p.x;
            L.y = ml.pos.y - isect.p.y;
            L.z = ml.pos.z - isect.p.z;
            ml_distance = L.normalize();
            float cos_N_L = L.dot(shadingN);
            if (cos_N_L > 0.f) {
                Ray shadowRay;
                Intersection Lsect;

                shadowRay = new Ray (isect.p, L, ml_distance, r.depth+1);
                Lsect = mScene.shadowTrace (shadowRay);
                if (!Lsect.intersected) {
                    RGB diffuseRad = new RGB(ml.rad);
                    diffuseRad.mult(isect.m.Kd);
                    diffuseRad.mult (cos_N_L);
                    rad.add(diffuseRad);
                }
            }
        }
        // ambient light
        rad.R += isect.m.Kd.R * ambient.R;
        rad.G += isect.m.Kd.G * ambient.G;
        rad.B += isect.m.Kd.B * ambient.B;
    } // end direct + ambient
    // specular reflection
    if ((!isect.m.Ks.isZero()) && (r.depth < MAX_DEPTH)) {
        Ray specRay;
        RGB specRad;
        Vect specDir, sym_vRay;
        float RV_dot;

        // compute specular reflection
        sym_vRay = r.dir.symmetric();
        RV_dot = 2.f * shadingN.dot(sym_vRay);
        specDir = new Vect(shadingN);
        specDir.mult (RV_dot);
        specDir.sub (sym_vRay);
        specDir.normalize();

        specRay = new Ray(isect.p, specDir, 1E10f, r.depth+1);
        specRad = mRT.RayV(specRay);
        specRad.mult(isect.m.Ks);
        rad.add(specRad);
    }
    return rad;
}

}
*/