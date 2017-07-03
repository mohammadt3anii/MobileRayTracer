//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADER_H
#define MOBILERAYTRACER_SHADER_H

#include "Scene.h"
#include "RGB.h"
#include "Intersection.h"
#include "Ray.h"
#include "Vector3D.h"

namespace MobileRT {
    class Shader {
    public:
        Scene &scene_;

    protected:
        const unsigned int samplesLight_;

    protected:
        virtual void shade(RGB &, const Intersection &, const Ray &) const noexcept = 0;

    public:
        explicit Shader(Scene &scene, const unsigned int samplesLight) noexcept;

        virtual ~Shader(void) noexcept;

        void rayTrace(RGB &rgb, const Ray &ray, Intersection &intersection) const noexcept;

        int traceTouch(Intersection &intersection, const Ray &ray) const noexcept;

        virtual void resetSampling(void) noexcept = 0;
    };
}

#endif //MOBILERAYTRACER_SHADER_H
