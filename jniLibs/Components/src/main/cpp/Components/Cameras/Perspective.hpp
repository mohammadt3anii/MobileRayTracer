//
// Created by Tiago on 16-10-2016.
//

#ifndef COMPONENTS_CAMERAS_PERSPECTIVE_HPP
#define COMPONENTS_CAMERAS_PERSPECTIVE_HPP

#include "MobileRT/Camera.hpp"

namespace Components {
	using MobileRT::Point3D;
	using MobileRT::Vector3D;
	using MobileRT::Ray;
    class Perspective final : public MobileRT::Camera {
    private:
      const float hFov_ {};
      const float vFov_ {};

	private:
		float fastArcTan(float value) const noexcept;

    public:
			explicit Perspective (Point3D position,
														Point3D lookAt, Vector3D up,
														float hFov, float vFov) noexcept;

		Perspective(const Perspective &perspective) noexcept = delete;

		Perspective(Perspective &&perspective) noexcept = delete;

        ~Perspective() noexcept final = default;

		Perspective &operator=(const Perspective &perspective) const noexcept = delete;

		Perspective &&operator=(Perspective &&perspective) const noexcept = delete;

        Ray generateRay(float u, float v,
                                      float deviationU,
                                      float deviationV) const noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_CAMERAS_PERSPECTIVE_HPP
