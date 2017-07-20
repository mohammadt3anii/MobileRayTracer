//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_SHADER_HPP
#define MOBILERT_SHADER_HPP

#include "Intersection.hpp"
#include "RGB.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "Vector3D.hpp"

namespace MobileRT {
    class Shader {
    public:
        Scene &scene_;

    protected:
        const unsigned int samplesLight_;

    protected:
			virtual void shade(RGB & /*rgb*/, Intersection const & /*intersection*/, Ray && /*ray*/) const noexcept = 0;

		public:
        explicit Shader(Scene &&scene, unsigned int samplesLight) noexcept;

		Shader(const Shader &shader) noexcept = delete;

		Shader(Shader &&shader) noexcept = delete;

        virtual ~Shader() noexcept = default;

		Shader &operator=(const Shader &shader) noexcept = delete;

		Shader &operator=(Shader &&shader) noexcept = delete;

		//ray trace and verifies if intersects primitives
		template<typename T>
		void rayTrace(RGB &rgb, T &&intersection, Ray &&ray) const noexcept {
			if (this->scene_.trace(intersection, ray) >= 0)
			{
				shade(rgb, intersection, std::move(ray)); // compute radiance
			}
		}

        int traceTouch(Intersection &intersection, const Ray &ray) const noexcept;

        virtual void resetSampling() noexcept = 0;
    };
}//namespace MobileRT

#endif //MOBILERT_SHADER_HPP
