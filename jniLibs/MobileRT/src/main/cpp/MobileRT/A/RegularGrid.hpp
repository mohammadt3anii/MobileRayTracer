//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERT_ACCELERATORS_REGULARGRID_HPP
#define MOBILERT_ACCELERATORS_REGULARGRID_HPP

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
                             const std::vector<Primitive *> &primitives);

        bool intersect(Intersection &intersection, Ray &ray);
    };
}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_REGULARGRID_HPP
