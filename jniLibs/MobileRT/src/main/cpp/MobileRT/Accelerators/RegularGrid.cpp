//
// Created by Tiago on 6-Mar-17.
//

#include "RegularGrid.h"

using namespace MobileRT;

/*RegularGrid::RegularGrid(std::vector<Primitive *> primitives)
{
    float distMax (0.0f);
    for (Primitive *primitive1 : primitives)
    {
        for (Primitive *primitive2 : primitives)
        {
            if distance between point and other point > max
            max = distance between point and other point
        }
    }
}*/

RegularGrid::~RegularGrid(void)
{
}

void RegularGrid::build(void)
{
}

bool RegularGrid::intersect(Intersection &, Ray &)
{
    return false;
}
