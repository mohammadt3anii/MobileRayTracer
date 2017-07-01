//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_POINTLIGHT_H
#define MOBILERAYTRACER_POINTLIGHT_H

#include "MobileRT/Light.h"

namespace Components {
    using namespace MobileRT;

    class PointLight final : public Light {
		private:
			Point3D position_;

		public:
			explicit PointLight(const Material &radiance, const Point3D &position);

			virtual const Point3D getPosition(void) override final;

			virtual void resetSampling(void) override final;

			virtual bool intersect(Intersection &intersection, const Ray &ray,
								const Material &material) const override final;
    };
}

#endif //MOBILERAYTRACER_POINTLIGHT_H
