//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADERWHITTED_H
#define MOBILERAYTRACER_SHADERWHITTED_H

#include "Shader.h"

namespace MobileRT {
    class ShaderWhitted : public Shader {
    private:
        enum{
            MAX_DEPTH = 5
        };

    public:

        RGB ambient;

        ShaderWhitted(RayTrace &rayTrace, Scene &scene);

        virtual RGB Shade(const Ray &ray, const Intersection &intersection);
    };
}

#endif //MOBILERAYTRACER_SHADERWHITTED_H