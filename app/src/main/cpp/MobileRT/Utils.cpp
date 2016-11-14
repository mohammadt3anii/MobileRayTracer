//
// Created by Tiago on 14-11-2016.
//

#include "Utils.h"
#include "Constants.h"

float fastArcTan (const float value)
{
    const float abs ((value < 0)? -value : value);
    return M_PI_4*value - (value *(abs - 1)) * (0.2447 + (0.0663 * abs));
}