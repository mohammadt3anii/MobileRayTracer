//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_MATERIAL_H
#define MOBILERAYTRACER_MATERIAL_H

#include "RGB.h"

namespace MobileRT {
    class Material {
    public:
        const RGB Kd_;   // diffuse reflection
        const RGB Ks_;   // specular reflection

    public:
        explicit Material(void);

        explicit Material(const RGB &Kd);

        explicit Material(const RGB &Kd, const RGB &Ks);

        static unsigned int getInstances();
    };
}

#endif //MOBILERAYTRACER_MATERIAL_H
