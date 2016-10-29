//
// Created by puscas on 16-10-2016.
//

#include "ShaderWhitted.h"
#include "Constants.h"

using namespace MobileRT;

ShaderWhitted::ShaderWhitted (RayTrace* mRT, Scene mScene) : Shader(mRT, mScene)
{
}

RGB* ShaderWhitted::Shade (const Ray& r, const Intersection& isect)
{
    RGB* rad = new RGB();
    Vect shadingN;
    float cosRN = r.dir->dot(isect.normal());

    // the normal always points to outside objects (e.g., spheres)
    // if the cosine between the ray and the normal is less than 0 then
    // the ray intersected the object from the inside and the shading normal
    // should be symmetric to the geometric normal
    if (cosRN<0.f) { // entering the object
        shadingN = Vect(isect.normal());
    } else {
        // We have to reverse the normal now
        shadingN = isect.normal().symmetric();
    }


    // shadowed direct lighting - only for diffuse materials
    if (!isect.material()->Kd->isZero()) {
        int l;
        int Nl = mScene.lights.size();

        for (l=0 ; l < Nl ; l++) {
            Light* ml = mScene.lights[l];
            float ml_distance;

            Vect L(ml->pos - isect.point());
            ml_distance = L.normalize();
            float cos_N_L = L.dot(shadingN);
            if (cos_N_L > 0.f) {
                Intersection* Lsect;

                Point p = isect.point();
                Ray shadowRay(&p, &L, ml_distance, r.depth+1);
                Lsect = mScene.shadowTrace (shadowRay);
                if (!Lsect->intersected()) {
                    RGB* diffuseRad = new RGB(ml->rad);
                    diffuseRad->mult(*isect.material()->Kd);
                    diffuseRad->mult (cos_N_L);
                    rad->add(*diffuseRad);
                }
            }
        }
        // ambient light
        rad->R += isect.material()->Kd->R * ambient.R;
        rad->G += isect.material()->Kd->G * ambient.G;
        rad->B += isect.material()->Kd->B * ambient.B;
    } // end direct + ambient
    // specular reflection
    if ((!isect.material()->Ks->isZero()) && (r.depth < MAX_DEPTH)) {
        Ray* specRay;
        RGB* specRad;
        Vect specDir;
        Vect sym_vRay;
        float RV_dot;

        // compute specular reflection
        sym_vRay = r.dir->symmetric();
        RV_dot = 2.f * shadingN.dot(sym_vRay);
        specDir = shadingN;
        specDir.mult (RV_dot);
        specDir.sub (sym_vRay);
        specDir.normalize();

        Point p = isect.point();
        specRay = new Ray(&p, &specDir, MAX_LENGTH, r.depth+1);
        specRad = mRT->RayV(*specRay);
        specRad->mult(*isect.material()->Ks);
        rad->add(*specRad);
    }
    return rad;
}