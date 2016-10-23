//
// Created by puscas on 16-10-2016.
//

#include "myPoint.h"

using namespace MobileRT;

myPoint::myPoint () : x(.0f), y(.0f), z(.0f) {
}

myPoint::myPoint (const float px, const float py, const float pz) :
    x(px), y(py), z(pz) {
}

myPoint::myPoint (const myPoint& p) :
    x(p.x), y(p.y), z(p.z) {
}