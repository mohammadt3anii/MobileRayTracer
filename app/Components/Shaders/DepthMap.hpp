//
// Created by Tiago on 26-07-2017.
//

#ifndef COMPONENTS_SHADERS_DEPTHMAP_HPP
#define COMPONENTS_SHADERS_DEPTHMAP_HPP

#include "MobileRT/Shader.hpp"

namespace Components {
    using ::MobileRT::Intersection;
    using ::MobileRT::Ray;
    using ::MobileRT::Scene;

    class DepthMap final : public ::MobileRT::Shader {
    private:
        ::glm::vec3 maxPoint_{};

    private:
        bool shade(::glm::vec3 *rgb, Intersection intersection,
                   Ray ray) noexcept final;

    public:
        explicit DepthMap(Scene scene, ::glm::vec3 maxPoint,
                          Accelerator accelerator) noexcept;

        DepthMap(const DepthMap &depthMap) noexcept = delete;

        DepthMap(DepthMap &&depthMap) noexcept = delete;

        ~DepthMap() noexcept final = default;

        DepthMap &operator=(const DepthMap &depthMap) noexcept = delete;

        DepthMap &operator=(DepthMap &&depthMap) noexcept = delete;

        void resetSampling() noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_SHADERS_DEPTHMAP_HPP
