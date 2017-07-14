//
// Created by puscas on 06-03-2017.
//

#ifndef MOBILERAYTRACER_AXISALIGNEDBOUNDINGBOX_H
#define MOBILERAYTRACER_AXISALIGNEDBOUNDINGBOX_H

#include "../Intersection.hpp"
#include "../Material.hpp"
#include "../Primitive.hpp"
#include "../Ray.hpp"
#include "../Utils.hpp"
#include <vector>

namespace MobileRT {
    class AxisAlignedBoundingBox final {
    private:
        const Point3D pointMin_;
        const Point3D pointMax_;

    public:
        std::vector<Primitive *> primitives_;

    private:
        bool intersectPrimitives(Intersection &intersection, const Ray &ray) const;

    public:
        explicit AxisAlignedBoundingBox(const Point3D &pointA,
                                        const Point3D &pointH);

        bool intersect(Intersection &intersection, const Ray &ray,
                       const Material &material) const;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_AXISALIGNEDBOUNDINGBOX_H
