//
// Created by Tiago on 16-10-2016.
//

#include "Material.hpp"
#include <utility>

using ::MobileRT::Material;

Material::Material(::glm::vec3 Kd, ::glm::vec3 Ks, ::glm::vec3 Kt, const float refractiveIndice, ::glm::vec3 Le) noexcept :
        Le_{Le},
        Kd_{Kd},
        Ks_{Ks},
        Kt_{Kt},
        refractiveIndice_{refractiveIndice} {
}
