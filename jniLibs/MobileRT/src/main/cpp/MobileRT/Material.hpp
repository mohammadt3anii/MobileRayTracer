//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_MATERIAL_H
#define MOBILERAYTRACER_MATERIAL_H

#include "RGB.hpp"

namespace MobileRT {
    class Material final {
    public:
        const RGB Kd_;   // diffuse reflection
        const RGB Ks_;   // specular reflection
        const RGB Kt_;   // specular transmission
        const RGB Le_;   // auto emission
        const float refractiveIndice_;

    public:
        explicit Material() noexcept;

        explicit Material(const RGB &Kd,
                          float refractiveIndice = 1.0f) noexcept;

        explicit Material(const RGB &Kd, const RGB &Ks,
                          float refractiveIndice = 1.0f) noexcept;

        explicit Material(const RGB &Kd, const RGB &Ks, const RGB &Kt,
                          float refractiveIndice = 1.0f) noexcept;

        explicit Material(const RGB &Kd, const RGB &Ks, const RGB &Kt, const RGB &Le,
                          float refractiveIndice = 1.0f) noexcept;

        static unsigned int getInstances() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_MATERIAL_H
