//
// Created by puscas on 16-10-2016.
//

#include "Intersection.h"
#include "Constants.h"

using namespace MobileRT;

// constructor for no intersection
Intersection::Intersection () :
    intersected(false),
    t(MAX_T),
    m(nullptr)
{
}