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
        virtual void shade(RGB &, Intersection &, Ray &) const;

    public:
        explicit Shader(Scene &scene, const unsigned int samplesLight);

        virtual ~Shader(void);

        void rayTrace(RGB &rgb, Ray &ray, Intersection &intersection) const;

        int traceTouch(Intersection &intersection, Ray &ray) const;

        virtual void resetSampling(void);
    };
}

#endif //MOBILERAYTRACER_SHADER_H
