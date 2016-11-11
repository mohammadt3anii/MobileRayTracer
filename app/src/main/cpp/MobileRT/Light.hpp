//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_LIGHT_HPP
#define MOBILERAYTRACER_LIGHT_HPP

#include "RGB.hpp"
#include "Point.hpp"

namespace MobileRT
{
    class Light
    {
        private:
            

        public:
            const RGB rad;
            const Point pos;
            
            Light (const RGB& prad, const Point& ppos);
    };

}

#endif //MOBILERAYTRACER_LIGHT_HPP