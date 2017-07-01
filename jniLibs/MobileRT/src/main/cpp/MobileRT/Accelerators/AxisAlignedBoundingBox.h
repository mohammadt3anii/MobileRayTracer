//
// Created by puscas on 06-03-2017.
//

#ifndef MOBILERAYTRACER_AXISALIGNEDBOUNDINGBOX_H
#define MOBILERAYTRACER_AXISALIGNEDBOUNDINGBOX_H

#include "../Utils.h"
#include <vector>
#include "../Intersection.h"
#include "../Ray.h"
#include "../Material.h"
#include "../Primitive.h"

namespace MobileRT {
    class AxisAlignedBoundingBox final {
    public:
        const Point3D pointMin_;
        const Point3D pointMax_;

    public:
        //std::vector<Primitive *> *primitives_;

        public:
            explicit AxisAlignedBoundingBox(const Point3D &pointA,
											const Point3D &pointH);

            bool intersect(Intersection &intersection, const Ray &ray,
                           const Material &material) const;
    };
}

#endif //MOBILERAYTRACER_AXISALIGNEDBOUNDINGBOX_H
