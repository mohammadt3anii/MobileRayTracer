//
// Created by Tiago on 14/01/2017.
//

#ifndef MOBILERAYTRACER_REGULARGRID_H
#define MOBILERAYTRACER_REGULARGRID_H

#include <vector>
#include "../Primitive.h"
#include "AxisAlignedBoundingBox.h"

namespace MobileRT {
    class RegularGrid {
        private:
            std::vector<AxisAlignedBoundingBox *> *boxes_;

    	private:
        	void divideBoundingBox(AxisAlignedBoundingBox &bb);

        public:
            explicit RegularGrid(const float minX, const float maxX,
                                 const float minY, const float maxY,
                                 const float minZ, const float maxZ,
                                 const unsigned int numberOfBlocks,
                                 std::vector<Primitive *> primitives);

			~RegularGrid(void);

			bool intersect(Intersection &intersection, Ray &ray);
    };
}

#endif //MOBILERAYTRACER_REGULARGRID_H
