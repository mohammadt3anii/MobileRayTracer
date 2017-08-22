//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_SHADER_HPP
#define MOBILERT_SHADER_HPP

#include "Accelerators/RegularGrid.hpp"
#include "Intersection.hpp"
#include "RGB.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "Vector3D.hpp"
#include <utility>

namespace MobileRT {
  class Shader {
    public:
    Scene scene_;
    RegularGrid regularGrid_;
    enum Accelerator {
      NONE = 0,
      REGULAR_GRID
    };
    private:
    const Accelerator accelerator_ {};

    protected:
			const unsigned samplesLight_ {};

    protected:
			virtual bool shade (RGB *rgb, Intersection intersection,
                          Ray &&ray) const noexcept = 0;
    private:
      template<typename T>
      void getSceneBounds(std::vector<T> primitives, Point3D *min, Point3D *max);
    void AABBbounds (const AABB box, Point3D *const min, Point3D *const max);
    public:
    void initializeGrid (AABB camera) noexcept;

		public:
    explicit Shader (Scene &&scene, unsigned samplesLight, Accelerator accelerator) noexcept;

			Shader(const Shader &shader) noexcept = delete;

			Shader(Shader &&shader) noexcept = delete;

			virtual ~Shader() noexcept;

			Shader &operator=(const Shader &shader) noexcept = delete;

			Shader &operator=(Shader &&shader) noexcept = delete;

			bool rayTrace(RGB *rgb, Intersection *intersection, Ray &&ray) const noexcept;

      int traceTouch(Intersection *intersection, Ray &&ray) const noexcept;

			virtual void resetSampling() noexcept = 0;
  };
}//namespace MobileRT

#endif //MOBILERT_SHADER_HPP
