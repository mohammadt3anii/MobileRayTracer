//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_MATERIAL_HPP
#define MOBILERT_MATERIAL_HPP

#include "RGB.hpp"

namespace MobileRT {
    class Material final {
    public:
		const RGB Le_;   // auto emission
        const RGB Kd_;   // diffuse reflection
        const RGB Ks_;   // specular reflection
        const RGB Kt_;   // specular transmission
		const float refractiveIndice_;

    public:
        explicit Material(RGB Kd = RGB(), RGB Ks = RGB(), RGB Kt = RGB(), float refractiveIndice = 1.0f, RGB Le = RGB()) noexcept;

		Material(const Material &material) noexcept = default;

		Material(Material &&material) noexcept = delete;

        ~Material() noexcept = default;

		Material &operator=(const Material &material) noexcept = delete;

		Material &operator=(Material &&material) noexcept = delete;

        static unsigned int getInstances() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_MATERIAL_HPP
