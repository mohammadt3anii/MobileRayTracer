//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADER_H
#define MOBILERAYTRACER_SHADER_H

#include "../Scene.h"
#include "../RGB.h"
#include "../Intersection.h"
#include "../Ray.h"
#include "../Vector3D.h"

namespace MobileRT {
    class Shader {
    protected:
        const Scene &scene_;

    protected:
        virtual void shade(RGB &, Intersection &, const Ray &) const;

    public:
        explicit Shader(const Scene &scene);

        virtual ~Shader(void);

        void rayTrace(RGB &rgb, Ray &ray, Intersection &intersection) const;
        int traceTouch(Intersection &intersection, Ray &ray) const;
    };
}

#endif //MOBILERAYTRACER_SHADER_H
