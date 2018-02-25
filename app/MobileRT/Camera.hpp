//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERT_CAMERA_HPP
#define MOBILERT_CAMERA_HPP

#include "Accelerators/AABB.hpp"
#include "Ray.hpp"
#include <algorithm>
#include <atomic>
#include <glm/glm.hpp>
#include <random>

namespace MobileRT {
    using ::MobileRT::AABB;

    class Camera {
    protected:
        ::std::atomic<uint32_t> block_{0};

    public:
        ::glm::vec3 position_{};
        ::glm::vec3 direction_{};
        ::glm::vec3 right_{};
        ::glm::vec3 up_{};

    public:
        explicit Camera(::glm::vec3 position,
                        ::glm::vec3 lookAt, ::glm::vec3 up) noexcept;

        Camera(const Camera &camera) noexcept;

        Camera(Camera &&camera) noexcept = delete;

        virtual ~Camera() noexcept;

        Camera &operator=(const Camera &camera) noexcept = delete;

        Camera &operator=(Camera &&camera) noexcept = delete;

        virtual Ray generateRay(float u, float v,
                                float deviationU,
                                float deviationV) const noexcept = 0;

        float getBlock(uint32_t sample) noexcept;

        virtual AABB getAABB() const noexcept;

        void resetSampling() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_CAMERA_HPP
