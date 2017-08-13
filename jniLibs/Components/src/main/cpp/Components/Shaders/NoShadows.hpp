//
// Created by Tiago on 16-10-2016.
//

#ifndef COMPONENTS_SHADERS_NOSHADOWS_HPP
#define COMPONENTS_SHADERS_NOSHADOWS_HPP

#include "MobileRT/Shader.hpp"

namespace Components {
	using MobileRT::RGB;
	using MobileRT::Intersection;
	using MobileRT::Ray;
	using MobileRT::Scene;
    class NoShadows final : public MobileRT::Shader {
    private:
			bool shade(RGB *rgb,
								 Intersection intersection,
								 Ray &&ray) const noexcept final;

		public:
			explicit NoShadows (Scene &&scene, unsigned samplesLight) noexcept;
			NoShadows (const NoShadows &noShadows) noexcept = delete;
			NoShadows (NoShadows &&noShadows) noexcept = delete;
			~NoShadows () noexcept final = default;
			NoShadows &operator= (const NoShadows &noShadows) noexcept = delete;
			NoShadows &operator= (NoShadows &&noShadows) noexcept = delete;
			void resetSampling () noexcept final;
    };
}//namespace Components

#endif //COMPONENTS_SHADERS_NOSHADOWS_HPP
