//
// Created by Tiago on 22-Jun-17.
//

#ifndef COMPONENTS_CAMERAS_ORTHOGRAPHIC_HPP
#define COMPONENTS_CAMERAS_ORTHOGRAPHIC_HPP

#include "MobileRT/Camera.hpp"

namespace Components {
	using MobileRT::Point3D;
	using MobileRT::Vector3D;
	using MobileRT::Ray;
    class Orthographic final : public MobileRT::Camera {
    private:
      const float sizeH_ {};
      const float sizeV_ {};

    public:
			explicit Orthographic (Point3D position,
														 Point3D lookAt, Vector3D up,
														 float sizeH, float sizeV) noexcept;

		Orthographic(const Orthographic &orthographic) noexcept = delete;

		Orthographic(Orthographic &&orthographic) noexcept = delete;

        ~Orthographic() noexcept final = default;

        Orthographic &operator=(const Orthographic &orthographic) noexcept = delete;

		Orthographic &operator=(Orthographic &&orthographic) noexcept = delete;

        Ray generateRay(float u, float v,
                                      float deviationU,
                                      float deviationV) const noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_CAMERAS_ORTHOGRAPHIC_HPP
