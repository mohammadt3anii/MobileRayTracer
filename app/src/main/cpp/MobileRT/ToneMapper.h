//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_TONEMAPPER_H
#define MOBILERAYTRACER_TONEMAPPER_H

#include <array>
#include "RGB.h"

namespace MobileRT {
    class ToneMapper {
    private:
        float Dlimit = 256.f;
        int Ilimit = 255;

    public:
        ToneMapper ();
        std::array<unsigned char, 1>* RGB2Color (RGB px);
    };
}

#endif //MOBILERAYTRACER_TONEMAPPER_H