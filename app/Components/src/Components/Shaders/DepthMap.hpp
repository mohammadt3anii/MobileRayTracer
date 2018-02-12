//
// Created by Tiago on 26-07-2017.
//

#ifndef COMPONENTS_SHADERS_DEPTHMAP_HPP
#define COMPONENTS_SHADERS_DEPTHMAP_HPP

#include "MobileRT/Shader.hpp"

namespace Components {
    using ::MobileRT::RGB;
    using ::MobileRT::Intersection;
    using ::MobileRT::Ray;
    using ::MobileRT::Scene;
    using ::MobileRT::Point3D;

    class DepthMap final : public ::MobileRT::Shader {
    private:
        Point3D maxPoint_{};

    private:
        bool shade(RGB *rgb,
                   Intersection intersection,
                   Ray &&ray) noexcept final;

    public:
        explicit DepthMap(Scene &&scene, Point3D maxPoint,
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
