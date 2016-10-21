//
// Created by puscas on 16-10-2016.
//

#include "ToneMapper.h"
#include <array>

using namespace MobileRT;

ToneMapper::ToneMapper () {

};

std::array<unsigned char, 1>* ToneMapper::RGB2Color (RGB px) {
    int r, g, b;
    std::array<unsigned char, 1>* p = new std::array<unsigned char, 1> ();

    // convert to int
    r = (int)(px.R*Dlimit);
    g = (int)(px.G*Dlimit);
    b = (int)(px.B*Dlimit);
    // clamp above
    r = (r>Ilimit ? Ilimit : r);
    g = (g>Ilimit ? Ilimit : g);
    b = (b>Ilimit ? Ilimit : b);
    //p.setARGB (255, (r<0? 0 : r), (g<0? 0 : g), (b<0? 0 : b));
    return p;
}