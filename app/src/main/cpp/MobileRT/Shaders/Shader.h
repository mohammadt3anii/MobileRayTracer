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
        typedef std::function<void(RGB &, Ray &, Intersection &, Vector3D &)> RayTraceCall;
        RayTraceCall rayTraceCall_;
        const Scene &scene_;

    public:
        Shader(const Scene &scene);

        virtual ~Shader();

        virtual void shade(RGB &, Intersection &, const Ray &, Vector3D &) const;

        void setRayTraceFunction(const RayTraceCall &rayTraceCall);
    };
}

#endif //MOBILERAYTRACER_SHADER_H