//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_MYMATERIAL_H
#define MOBILERAYTRACER_MYMATERIAL_H

#include "RGB.h"

namespace MobileRT {

    class Material {
        private:

        public:
            RGB* Kd;   // diffuse reflection
            RGB* Ks;   // specular reflection

        Material ();
        Material (RGB* pKd);
        Material (RGB* pKd, RGB* pKs);
    };
}
#endif //MOBILERAYTRACER_MYMATERIAL_H