//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERAYTRACER_LIGHT_H
#define MOBILERAYTRACER_LIGHT_H

#include "Intersection.hpp"
#include "Point3D.hpp"
#include "RGB.hpp"
#include "Ray.hpp"

namespace MobileRT {
    class Light {
    public:
        const Material radiance_;

    public:
        explicit Light(Material radiance) noexcept;

		Light(const Light &light) noexcept = default;

		Light(Light &&light) noexcept = default;

        virtual ~Light() noexcept = default;

		Light &operator=(const Light &light) const noexcept = delete;

		Light &&operator=(Light &&light) const noexcept = delete;

        virtual const Point3D getPosition() noexcept = 0;

        virtual void resetSampling() noexcept = 0;

        virtual bool intersectL(Intersection &intersection, const Ray &ray) const noexcept = 0;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_LIGHT_H
