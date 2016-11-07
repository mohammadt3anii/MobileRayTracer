//
// Created by puscas on 16-10-2016.
//

#include "ToneMapper.h"

using namespace MobileRT;

uint32_t ToneMapper::RGB2Color (const RGB& pixel)
{
    float r = pixel.R >1.0f ? 1.0f : (pixel.R < 0.0f ? 0.0f : pixel.R);
    float g = pixel.G >1.0f ? 1.0f : (pixel.G < 0.0f ? 0.0f : pixel.G);
    float b = pixel.B >1.0f ? 1.0f : (pixel.B < 0.0f ? 0.0f : pixel.B);

    unsigned char cr = static_cast<uint8_t> (r * 255);
    unsigned char cg = static_cast<uint8_t> (g * 255);
    unsigned char cb = static_cast<uint8_t> (b * 255);
    return 0xFF000000 | (cb << 16) | (cg << 8) | cr;
}