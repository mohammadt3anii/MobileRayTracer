//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_MATERIAL_HPP
#define MOBILERAYTRACER_MATERIAL_HPP

#include "RGB.hpp"

namespace MobileRT
{
    class Material
    {
        private:
            

        public:
            const RGB Kd;   // diffuse reflection
            const RGB Ks;   // specular reflection
            
            Material ();
            Material (const RGB& pKd);
            Material (const RGB& pKd, const RGB& pKs);
    };
}

#endif //MOBILERAYTRACER_MATERIAL_HPP