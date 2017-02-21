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

        virtual void shade(RGB &, Intersection &, const Ray &) const;

        RGB *accumulate_;
        unsigned int *bitmap_;
        float hfov_;
        float vfov_;
        unsigned int width_;
        unsigned int height_;

    public:
        explicit Shader(const Scene &scene);

        virtual ~Shader(void);

        void rayTrace(RGB &rgb, Ray &ray, Intersection &intersection) const;
        int traceTouch(Intersection &intersection, Ray &ray) const;

        void setBitmap(unsigned int *bitmap, RGB *accumulate, const float hfov, const float vfov,
                       const unsigned int width, const unsigned int height);
    };
}

#endif //MOBILERAYTRACER_SHADER_H
