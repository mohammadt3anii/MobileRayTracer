//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_LIGHT_H
#define MOBILERAYTRACER_LIGHT_H

#include "RGB.h"
#include "myPoint.h"

namespace MobileRT {
    class Light {
    private:

    public:
        const RGB rad;
        const myPoint pos;

        Light (const RGB& prad, const myPoint& ppos);
    };

}

#endif //MOBILERAYTRACER_LIGHT_H