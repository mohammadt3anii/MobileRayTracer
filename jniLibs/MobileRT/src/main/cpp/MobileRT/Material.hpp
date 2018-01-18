//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_MATERIAL_HPP
#define MOBILERT_MATERIAL_HPP

#include "RGB.hpp"

namespace MobileRT {
  class Material final {
    public:
    RGB Le_ {};   // auto emission
    RGB Kd_ {};   // diffuse reflection
    RGB Ks_ {};   // specular reflection
    RGB Kt_ {};   // specular transmission
    float refractiveIndice_ {};

    public:
      explicit Material (RGB Kd = RGB {}, RGB Ks = RGB {}, RGB Kt = RGB {},
                         float refractiveIndice = 1.0f, RGB Le = RGB {}) noexcept;

      Material (const Material &material) noexcept = default;

      Material (Material &&material) noexcept = default;
    ~Material () noexcept = default;
    Material &operator= (const Material &material) noexcept = default;
    Material &operator= (Material &&material) noexcept = default;
    static unsigned getInstances () noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_MATERIAL_HPP
