//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_NOSHADOWS_H
#define MOBILERAYTRACER_NOSHADOWS_H

#include "MobileRT/Shader.h"

namespace Components {
    using namespace MobileRT;

    class NoShadows final : public Shader {
    private:
        virtual void shade(RGB &rgb, Intersection &intersection, const Ray &) const override final;

    public:
        explicit NoShadows(Scene &scene, const unsigned int samplesLight);
    };
}

#endif //MOBILERAYTRACER_NOSHADOWS_H
