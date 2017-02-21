//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_WHITTED_H
#define MOBILERAYTRACER_WHITTED_H

#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Shaders/Shader.h"

namespace MobileRT {
    class Whitted : public Shader {
    private:
        void shade(RGB &rgb,
                   Intersection &intersection,
                   const Ray &ray) const override;

    public:
        explicit Whitted(const Scene &scene);
    };
}

#endif //MOBILERAYTRACER_WHITTED_H
