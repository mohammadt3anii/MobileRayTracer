//
// Created by Tiago on 22-Jun-17.
//

#ifndef MOBILERAYTRACER_ORTHOGRAPHIC_H
#define MOBILERAYTRACER_ORTHOGRAPHIC_H

#include "MobileRT/Camera.h"

namespace Components {
    using namespace MobileRT;

    class Orthographic final : public Camera {
		private:
			const float sizeH_;
			const float sizeV_;

		public:
			explicit Orthographic(const Point3D &position, const Point3D &lookAt,
								const Vector3D &up, const float sizeH, const float sizeV);

			virtual ~Orthographic(void) override final;

			virtual const Ray generateRay(const float x, const float y,
										const float deviationU,
										const float deviationV) const override final;
    };
}

#endif //MOBILERAYTRACER_ORTHOGRAPHIC_H
