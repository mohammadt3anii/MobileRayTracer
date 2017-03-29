//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_NOSHADOWS_H
#define MOBILERAYTRACER_NOSHADOWS_H

#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Shaders/Shader.h"

namespace MobileRT {
    class NoShadows : public Shader {
    private:
        void shade(RGB &rgb, Intersection &intersection, Ray &) const override;

    public:
        explicit NoShadows(Scene &scene, const unsigned int samplesLight);
    };
}

#endif //MOBILERAYTRACER_NOSHADOWS_H
