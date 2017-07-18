//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERT_LIGHT_HPP
#define MOBILERT_LIGHT_HPP

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

		Light(const Light &light) noexcept = delete;

		Light(Light &&light) noexcept = delete;

        virtual ~Light() noexcept = default;

		Light &operator=(const Light &light) const noexcept = delete;

		Light &&operator=(Light &&light) const noexcept = delete;

        virtual const Point3D getPosition() noexcept = 0;

        virtual void resetSampling() noexcept = 0;

        virtual bool intersect(Intersection &intersection, const Ray &ray) const noexcept = 0;
    };
}//namespace MobileRT

#endif //MOBILERT_LIGHT_HPP
