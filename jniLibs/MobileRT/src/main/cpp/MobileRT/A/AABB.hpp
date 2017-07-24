//
// Created by puscas on 06-03-2017.
//

#ifndef MOBILERT_ACCELERATORS_AABB_HPP
#define MOBILERT_ACCELERATORS_AABB_HPP

#include "../Intersection.hpp"
#include "../Material.hpp"
#include "../Ray.hpp"
#include "../Utils.hpp"
#include <vector>

namespace MobileRT {
    class AxisAlignedBoundingBox final {
    private:
        const Point3D pointMin_;
        const Point3D pointMax_;

    public:
        //std::vector<Primitive *> primitives_;

    private:
        bool intersectPrimitives(Intersection &intersection, const Ray &ray) const;

    public:
        explicit AxisAlignedBoundingBox(const Point3D &pointMin,
                                        const Point3D &pointMax);

		AxisAlignedBoundingBox(const AxisAlignedBoundingBox &axisAlignedBoundingBox) noexcept = delete;

		AxisAlignedBoundingBox(AxisAlignedBoundingBox &&axisAlignedBoundingBox) noexcept = delete;

		~AxisAlignedBoundingBox() noexcept = delete;

        AxisAlignedBoundingBox &operator=(const AxisAlignedBoundingBox &axisAlignedBoundingBox) noexcept = delete;

		AxisAlignedBoundingBox &operator=(AxisAlignedBoundingBox &&axisAlignedBoundingBox) noexcept = delete;

        bool intersect(Intersection &intersection, const Ray &ray,
                       const Material &material) const;
    };
}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_AABB_HPP
