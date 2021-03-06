//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_INTERSECTION_HPP
#define MOBILERT_INTERSECTION_HPP

#include "MobileRT/Material.hpp"
#include "MobileRT/Utils.hpp"
#include <glm/glm.hpp>

namespace MobileRT {
    class Intersection final {
    public:
        ::glm::vec3 point_{};   // intersection point
        ::glm::vec3 normal_{};    // intersection normal
        const Material *material_{nullptr};// material of the intersected primitive
        float length_{RayLengthMax};// ray length parameter
        const void *primitive_{nullptr};
        ::std::int32_t materialId_ {};
        ::glm::vec3 primitiveColor_ {};

    public:
        explicit Intersection () noexcept = delete;

        explicit Intersection(float dist) noexcept;

        explicit Intersection(
                const ::glm::vec3 &intPoint,
                float dist,
                const ::glm::vec3 &normal,
                const void *primitive, ::std::int32_t materialId,
                ::glm::vec3 primitiveColor = ::glm::vec3 {1}) noexcept;

        Intersection(const Intersection &intersection) noexcept = default;

        Intersection(Intersection &&intersection) noexcept = default;

        ~Intersection() noexcept = default;

        Intersection &operator=(const Intersection &intersection) noexcept = delete;

        Intersection &operator=(Intersection &&intersection) noexcept = default;
    };
}//namespace MobileRT

#endif //MOBILERT_INTERSECTION_HPP
