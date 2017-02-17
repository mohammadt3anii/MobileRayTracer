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
        virtual void shade(RGB &, Intersection &, const Ray &, Vector3D &) const;

    public:
        Shader(const Scene &scene);
        virtual ~Shader();
        void rayTrace(RGB &rgb, Ray &ray, Intersection &intersection, Vector3D &vector) const;
        int traceTouch(Intersection &intersection, Ray &ray) const;
    };
}

#endif //MOBILERAYTRACER_SHADER_H
