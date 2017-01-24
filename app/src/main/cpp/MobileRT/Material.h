//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_MATERIAL_H
#define MOBILERAYTRACER_MATERIAL_H

#include "RGB.h"

namespace MobileRT {
    class Material {
    private:


    public:
        const RGB Kd_;   // diffuse reflection
        const RGB Ks_;   // specular reflection

        Material();
        Material(const RGB &Kd);
        Material(const RGB &Kd, const RGB &Ks);
    };
}

#endif //MOBILERAYTRACER_MATERIAL_H