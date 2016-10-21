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
        ShaderNoShadows (RayTrace* mRT, Scene mScene);
        RGB* Shade (Ray r, Intersection isect);
    };
}

#endif //MOBILERAYTRACER_SHADERNOSHADOWS_H