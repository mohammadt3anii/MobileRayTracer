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
        const RGB Kt_;   // specular transmission
        const RGB Ke_;   // emission
        const float refractiveIndice_;

    public:
        explicit Material(void);
        explicit Material(const RGB &Kd);
        explicit Material(const RGB &Kd, const RGB &Ks);

        explicit Material(const RGB &Kd, const RGB &Ks, const RGB &Kt);

        explicit Material(const RGB &Kd, const RGB &Ks, const RGB &Kt, const RGB &Ke);

        explicit Material(const RGB &Kd, const float refractiveIndice);

        explicit Material(const RGB &Kd, const RGB &Ks, const float refractiveIndice);

        explicit Material(const RGB &Kd, const RGB &Ks, const RGB &Kt,
                          const float refractiveIndice);

        explicit Material(const RGB &Kd, const RGB &Ks, const RGB &Kt, const RGB &Ke,
                          const float refractiveIndice);

        static unsigned int getInstances();
    };
}

#endif //MOBILERAYTRACER_MATERIAL_H
