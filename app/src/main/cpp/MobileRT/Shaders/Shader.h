//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SHADER_H
#define MOBILERAYTRACER_SHADER_H

#include "../Scene.h"

namespace MobileRT {
    class Shader {
    protected:
        typedef std::function<void(RGB &, Ray &, Intersection &, Vector3D &)> RayTraceCall;
        const Scene &scene_;

    public:
        Shader(const Scene &scene);

        virtual ~Shader();

        virtual void shade(RGB &, Intersection &, const Ray &, Vector3D &, RayTraceCall) const;
    };
}

#endif //MOBILERAYTRACER_SHADER_H