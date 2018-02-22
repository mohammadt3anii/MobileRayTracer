//
// Created by Tiago on 16-10-2016.
//

#ifndef COMPONENTS_CAMERAS_PERSPECTIVE_HPP
#define COMPONENTS_CAMERAS_PERSPECTIVE_HPP

#include "MobileRT/Camera.hpp"

namespace Components {
    using ::MobileRT::Ray;

    class Perspective final : public ::MobileRT::Camera {
    private:
        float hFov_{};
        float vFov_{};

    private:
        float fastArcTan(float value) const noexcept;

    public:
        explicit Perspective(glm::vec3 position,
                             glm::vec3 lookAt, glm::vec3 up,
                             float hFov, float vFov) noexcept;

        Perspective(const Perspective &perspective) noexcept = default;

        Perspective(Perspective &&perspective) noexcept = delete;

        ~Perspective() noexcept final = default;

        Perspective &operator=(const Perspective &perspective) noexcept = delete;

        Perspective &operator=(Perspective &&perspective) noexcept = delete;

        Ray generateRay(float u, float v,
                        float deviationU, float deviationV) const noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_CAMERAS_PERSPECTIVE_HPP
