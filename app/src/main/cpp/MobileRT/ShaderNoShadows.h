//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADERNOSHADOWS_H
#define MOBILERAYTRACER_SHADERNOSHADOWS_H

#include "Shader.h"

namespace MobileRT{
    class ShaderNoShadows : public Shader {
    private:
    public:
        ShaderNoShadows(RayTrace &rayTrace, Scene &scene);

        virtual RGB Shade(const Ray &ray, const Intersection &intersection);
    };
}

#endif //MOBILERAYTRACER_SHADERNOSHADOWS_H