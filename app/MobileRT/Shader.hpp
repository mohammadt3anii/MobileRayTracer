//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_SHADER_HPP
#define MOBILERT_SHADER_HPP

#include "Accelerators/BVH.hpp"
#include "Accelerators/RegularGrid.hpp"
#include "Camera.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"
#include "Scene.hpp"

namespace MobileRT {
    class Shader {
    public:
        Scene scene_{};
        RegularGrid regularGrid_{};

        BVH<MobileRT::Plane> bvhPlanes_{};
        BVH<MobileRT::Rectangle> bvhRectangles_{};
        BVH<MobileRT::Sphere> bvhSpheres_{};
        BVH<MobileRT::Triangle> bvhTriangles_{};

        enum Accelerator {
            NAIVE = 0,
            REGULAR_GRID,
            BVH
        };

    private:
        const Accelerator accelerator_{};

    protected:
        const ::std::uint32_t samplesLight_{};

    protected:
        virtual bool shade(::glm::vec3 *rgb, Intersection intersection, Ray ray) noexcept = 0;

    public:
        void initializeAccelerators(Camera *camera) noexcept;

    public:
        explicit Shader(Scene scene, ::std::uint32_t samplesLight, Accelerator accelerator) noexcept;

        Shader(const Shader &shader) noexcept = delete;

        Shader(Shader &&shader) noexcept = default;

        virtual ~Shader() noexcept;

        Shader &operator=(const Shader &shader) noexcept = delete;

        Shader &operator=(Shader &&shader) noexcept = delete;

        bool rayTrace(::glm::vec3 *rgb, Ray ray) noexcept;

        bool shadowTrace(Intersection intersection, Ray ray) noexcept;

        Intersection traceTouch(Intersection intersection, Ray ray) noexcept;

        virtual void resetSampling() noexcept = 0;
    };
}//namespace MobileRT

#endif //MOBILERT_SHADER_HPP
