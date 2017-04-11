//
// Created by Tiago on 23-Jan-17.
//

#include "Light.h"

using namespace MobileRT;

Light::Light(const RGB &radiance) :
        radiance_(radiance)
{
}

/*Light::Light(const RGB &radiance, Shape *shape, const Material &material) :
        Primitive(shape, material),
        radiance_(radiance)
{
}*/

Light::~Light (void)
{
}
