//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_TONEMAPPER_H
#define MOBILERAYTRACER_TONEMAPPER_H

#include <array>
#include "RGB.h"

namespace MobileRT {
    class ToneMapper
    {
    public:
        static uint32_t RGB2Color (const RGB& px);
    };
}

#endif //MOBILERAYTRACER_TONEMAPPER_H