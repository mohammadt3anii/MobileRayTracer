//
// Created by Tiago on 26-07-2017.
//

#ifndef COMPONENTS_SHADERS_DEPTHMAP_HPP
#define COMPONENTS_SHADERS_DEPTHMAP_HPP

#include "MobileRT/Shader.hpp"

namespace Components {

    class DepthMap final : public ::MobileRT::Shader {
    private:
        ::glm::vec3 maxPoint_{};

    private:
        bool shade(
            ::glm::vec3 *rgb,
            ::MobileRT::Intersection intersection,
            ::MobileRT::Ray ray) noexcept final;

    public:
        explicit DepthMap(
            ::MobileRT::Scene scene,
            ::glm::vec3 maxPoint,
            Accelerator accelerator) noexcept;

        DepthMap(const DepthMap &depthMap) noexcept = delete;

        DepthMap(DepthMap &&depthMap) noexcept = delete;

        ~DepthMap() noexcept final = default;

        DepthMap &operator=(const DepthMap &depthMap) noexcept = delete;

        DepthMap &operator=(DepthMap &&depthMap) noexcept = delete;
    };
}//namespace Components

#endif //COMPONENTS_SHADERS_DEPTHMAP_HPP
