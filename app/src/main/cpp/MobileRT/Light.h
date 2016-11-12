//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_LIGHT_H
#define MOBILERAYTRACER_LIGHT_H

#include "RGB.h"
#include "Point.h"

namespace MobileRT
{
    class Light
    {
        private:
            

        public:
        const RGB rad_;
        const Point pos_;

        Light(const RGB &rad, const Point &pos);
    };
}

#endif //MOBILERAYTRACER_LIGHT_H