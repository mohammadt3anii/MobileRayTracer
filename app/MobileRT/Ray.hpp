//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_RAY_HPP
#define MOBILERT_RAY_HPP

#include "Utils.hpp"
#include <glm/glm.hpp>

namespace MobileRT {
    class Ray final {
    public:
        const glm::vec3 origin_ {};
        const glm::vec3 direction_  {};
        const int32_t depth_{};
        const int32_t id_ {};
        const void *const primitive_ {};

    public:
        explicit Ray(glm::vec3 dir, glm::vec3 origin,
                     int32_t depth, const void *primitive = nullptr) noexcept;

        Ray(const Ray &ray) noexcept = default;

        Ray(Ray &&ray) noexcept = default;

        ~Ray() noexcept = default;

        Ray &operator=(const Ray &ray) noexcept = delete;

        Ray &operator=(Ray &&ray) noexcept = delete;

        static unsigned getInstances() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_RAY_HPP
