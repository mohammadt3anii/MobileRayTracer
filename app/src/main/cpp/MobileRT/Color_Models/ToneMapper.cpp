//
// Created by Tiago on 16-10-2016.
//

#include "ToneMapper.h"

using namespace MobileRT;

unsigned int ToneMapper::RGB2Color(const RGB &pixel) {
    const float r((pixel.R_ > 1.0f) ? 1.0f : ((pixel.R_ < 0.0f) ? 0.0f : pixel.R_));
    const float g((pixel.G_ > 1.0f) ? 1.0f : ((pixel.G_ < 0.0f) ? 0.0f : pixel.G_));
    const float b((pixel.B_ > 1.0f) ? 1.0f : ((pixel.B_ < 0.0f) ? 0.0f : pixel.B_));

    const unsigned char cr(static_cast<unsigned char> (r * 255));
    const unsigned char cg(static_cast<unsigned char> (g * 255));
    const unsigned char cb(static_cast<unsigned char> (b * 255));
    return (0xFF000000 | (cb << 16) | (cg << 8) | cr);
}