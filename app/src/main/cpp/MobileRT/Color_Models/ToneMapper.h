//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_TONEMAPPER_H
#define MOBILERAYTRACER_TONEMAPPER_H

#include "RGB.h"

namespace MobileRT {
    class ToneMapper {
    private:


    public:
        static unsigned int RGB2Color(const RGB &pixel);
    };
}

#endif //MOBILERAYTRACER_TONEMAPPER_H