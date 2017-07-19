//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_PRIMITIVE_HPP
#define MOBILERT_PRIMITIVE_HPP

#include "Intersection.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include "Shapes/Plane.hpp"
#include "Shapes/Shape.hpp"
#include "Shapes/Sphere.hpp"
#include "Shapes/Triangle.hpp"

namespace MobileRT {
    class Primitive final {
    private:
        const Material material_;

    public:
        Shape *const shape_;

    public:
        explicit Primitive(Shape *shape, Material material) noexcept;

		Primitive(const Primitive &primitive) noexcept = delete;

		Primitive(Primitive &&primitive) noexcept = delete;

        ~Primitive() noexcept;

		Primitive &operator=(const Primitive &primitive) noexcept = delete;

		Primitive &operator=(Primitive &&primitive) noexcept = delete;

        bool intersect(Intersection &intersection, const Ray &ray) const noexcept;

        static unsigned int getInstances() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_PRIMITIVE_HPP
