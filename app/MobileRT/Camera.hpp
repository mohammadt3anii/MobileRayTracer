//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERT_CAMERA_HPP
#define MOBILERT_CAMERA_HPP

#include "Accelerators/AABB.hpp"
#include "Point3D.hpp"
#include "Ray.hpp"
#include <algorithm>
#include <atomic>
#include <random>

namespace MobileRT {
    using ::MobileRT::AABB;

    class Camera {
    protected:
        ::std::atomic<unsigned> block_{0};

    public:
        Point3D position_{};
        Vector3D direction_{};
        Vector3D right_{};
        Vector3D up_{};

    public:
        explicit Camera(Point3D position,
                        Point3D lookAt, Vector3D up) noexcept;

        Camera(const Camera &camera) noexcept;

        Camera(Camera &&camera) noexcept = delete;

        virtual ~Camera() noexcept;

        Camera &operator=(const Camera &camera) noexcept = delete;

        Camera &operator=(Camera &&camera) noexcept = delete;

        virtual Ray generateRay(float u, float v,
                                float deviationU,
                                float deviationV) const noexcept = 0;

        float getBlock(unsigned sample) noexcept;

        virtual AABB getAABB() const noexcept;

        void resetSampling() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_CAMERA_HPP
