//
// Created by Tiago on 16-10-2016.
//

#ifndef COMPONENTS_SHADERS_WHITTED_HPP
#define COMPONENTS_SHADERS_WHITTED_HPP

#include "MobileRT/Shader.hpp"

namespace Components {
    using ::MobileRT::RGB;
    using ::MobileRT::Intersection;
    using ::MobileRT::Ray;
    using ::MobileRT::Scene;

    class Whitted final : public ::MobileRT::Shader {
    private:
        bool shade(RGB *rgb,
                   Intersection intersection,
                   Ray &&ray) noexcept final;

    public:
        explicit Whitted(Scene &&scene, unsigned samplesLight, Accelerator accelerator) noexcept;

        Whitted(const Whitted &whitted) noexcept = delete;

        Whitted(Whitted &&whitted) noexcept = delete;

        ~Whitted() noexcept final = default;

        Whitted &operator=(const Whitted &whitted) noexcept = delete;

        Whitted &operator=(Whitted &&whitted) noexcept = delete;

        void resetSampling() noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_SHADERS_WHITTED_HPP
