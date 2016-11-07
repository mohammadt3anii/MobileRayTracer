//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_TONEMAPPER_H
#define MOBILERAYTRACER_TONEMAPPER_H

#include "RGB.h"
#include <stdint.h>

namespace MobileRT
{
    class ToneMapper
    {
        private:
            

        public:
            static uint32_t RGB2Color (const RGB& px);
    };
}

#endif //MOBILERAYTRACER_TONEMAPPER_H