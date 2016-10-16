//
// Created by puscas on 16-10-2016.
//

#include "myShape.h"

using namespace MobileRT;

myShape::myShape ()
{

}

Intersection* myShape::Intersect (Ray r) {
    return (new Intersection());
}