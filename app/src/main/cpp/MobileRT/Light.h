//
// Created by puscas on 16-10-2016.
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
            const RGB rad_;
            const Point pos_;

        public:
            Light (const RGB& prad, const Point& ppos);

            const RGB& getRad () const;
            const Point& getPos () const;
    };

}

#endif //MOBILERAYTRACER_LIGHT_H