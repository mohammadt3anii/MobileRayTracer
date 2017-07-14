//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERAYTRACER_REGULARGRID_H
#define MOBILERAYTRACER_REGULARGRID_H

#include "AABB.hpp"
#include "../Primitive.hpp"
#include <vector>

namespace MobileRT {
    class RegularGrid {
    private:
        //std::vector<AxisAlignedBoundingBox *> *boxes_;

    private:
        //void divideBoundingBox(AxisAlignedBoundingBox &bb);

    public:
        explicit RegularGrid(float minX, float maxX,
                             float minY, float maxY,
                             float minZ, float maxZ,
                             unsigned int numberOfBlocks,
                             std::vector<Primitive *> primitives);

        ~RegularGrid() = default;

        bool intersect(Intersection &intersection, Ray &ray);
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_REGULARGRID_H
