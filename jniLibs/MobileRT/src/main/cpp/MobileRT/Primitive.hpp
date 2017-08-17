//
// Created by puscas on 17/08/17.
//

#ifndef MOBILERT_PRIMITIVE_HPP
#define MOBILERT_PRIMITIVE_HPP

#include "Material.hpp"

namespace MobileRT {
  using MobileRT::Material;

  template<class T>
  class Primitive final {
    public:
    T shape_;
    Material material_;
    public:
    Primitive (T shape, Material material) noexcept :
      shape_ {shape},
      material_ {material} {
    }

    Primitive () noexcept = delete;
  };
}//namespace MobileRT

#endif //MOBILERT_PRIMITIVE_HPP
