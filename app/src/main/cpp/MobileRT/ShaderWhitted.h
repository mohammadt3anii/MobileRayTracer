//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADERWHITTED_H
#define MOBILERAYTRACER_SHADERWHITTED_H

#include "Shader.h"

namespace MobileRT
{
    class ShaderWhitted : public Shader
    {
        private:
            enum{
                MAX_DEPTH = 5
            };
            const RGB ambient_;

        public:
            ShaderWhitted(RayTrace &rayTrace, Scene &scene);

            RGB Shade(const Ray &ray, const Intersection &intersection) const override;
    };
}

#endif //MOBILERAYTRACER_SHADERWHITTED_H