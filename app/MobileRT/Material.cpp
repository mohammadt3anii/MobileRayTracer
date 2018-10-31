//
// Created by Tiago on 16-10-2016.
//

#include "MobileRT/Material.hpp"
#include "MobileRT/Utils.hpp"

using ::MobileRT::Material;

Material::Material(
        const ::glm::vec3 &Kd, const ::glm::vec3 &Ks,
        const ::glm::vec3 &Kt, const float refractiveIndice,
        const ::glm::vec3 &Le) noexcept :
        Le_{Le},
        Kd_{Kd},
        Ks_{Ks},
        Kt_{Kt},
        refractiveIndice_{refractiveIndice} {
}

bool Material::operator==(const Material &material) const noexcept {
    const bool sameLe {::glm::all(::glm::lessThanEqual(::glm::abs(this->Le_ - material.Le_), ::glm::vec3(Epsilon)))};
    const bool sameKd {::glm::all(::glm::lessThanEqual(::glm::abs(this->Kd_ - material.Kd_), ::glm::vec3(Epsilon)))};
    const bool sameKs {::glm::all(::glm::lessThanEqual(::glm::abs(this->Ks_ - material.Ks_), ::glm::vec3(Epsilon)))};
    const bool sameKt {::glm::all(::glm::lessThanEqual(::glm::abs(this->Kt_ - material.Kt_), ::glm::vec3(Epsilon)))};
    const bool sameRi {::std::abs(this->refractiveIndice_ - material.refractiveIndice_) < Epsilon};
    const bool res {sameLe && sameKd && sameKs && sameKt && sameRi};
    return res;
}
