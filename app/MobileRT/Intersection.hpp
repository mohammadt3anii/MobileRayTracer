//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_INTERSECTION_HPP
#define MOBILERT_INTERSECTION_HPP

#include "Material.hpp"
#include "Utils.hpp"
#include <glm/glm.hpp>

namespace MobileRT {
    class Intersection final {
    public:
        ::glm::vec3 point_{};   // intersection point
        ::glm::vec3 normal_{};    // intersection normal
        ::glm::vec3 symNormal_{};
        const Material *material_{nullptr};// material of the intersected primitive
        float length_{RayLengthMax};// ray length parameter
        const void *primitive_{nullptr};

    public:
        explicit Intersection() noexcept = default;

        explicit Intersection(const void *primitive) noexcept;

        explicit Intersection(::glm::vec3 orig,
                         ::glm::vec3 dir,
                         float dist,
                         ::glm::vec3 normal,
                         const void *primitive = nullptr) noexcept;

        Intersection(::glm::vec3 intPoint,
                         float dist, ::glm::vec3 center) noexcept;

        Intersection(const Intersection &intersection) noexcept = default;

        Intersection(Intersection &&intersection) noexcept = default;

        ~Intersection() noexcept = default;

        Intersection &operator=(const Intersection &intersection) noexcept = delete;

        Intersection &operator=(Intersection &&intersection) noexcept = default;
    };
}//namespace MobileRT

#endif //MOBILERT_INTERSECTION_HPP
