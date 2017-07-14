//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADER_H
#define MOBILERAYTRACER_SHADER_H

#include "Intersection.hpp"
#include "RGB.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "Vector3D.hpp"

namespace MobileRT {
    class Shader {
    public:
        Scene &scene_;

    protected:
        const unsigned int samplesLight_;

    protected:
        virtual void shade(RGB &/*rgb*/, const Intersection &/*intersection*/, const Ray &/*ray*/) const noexcept = 0;

    public:
        explicit Shader(Scene &scene, unsigned int samplesLight) noexcept;

        virtual ~Shader() noexcept = default;

        void rayTrace(RGB &rgb, const Ray &ray, Intersection &intersection) const noexcept;

        int traceTouch(Intersection &intersection, const Ray &ray) const noexcept;

        virtual void resetSampling() noexcept = 0;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_SHADER_H
