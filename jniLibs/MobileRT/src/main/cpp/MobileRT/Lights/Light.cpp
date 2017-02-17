//
// Created by Tiago on 23-Jan-17.
//

#include "Light.h"

using namespace MobileRT;

Light::Light(const RGB &radiance, const Point3D &position) :
        radiance_(radiance),
        position_(position) {
}
