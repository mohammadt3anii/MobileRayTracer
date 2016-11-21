//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_SAMPLER_H
#define MOBILERAYTRACER_SAMPLER_H

#include "../Scenes/Scene.h"
#include "../RayTracer.h"
#include "../Cameras/Perspective.h"

namespace MobileRT {
    class Sampler {
    protected:
        const unsigned int width_;
        const unsigned int height_;
        Scene *scene_;
        RayTracer *rayTracer_;
        Perspective *camera_;
        const unsigned int samples_;

    public:
        Sampler(const unsigned int width, const unsigned int height,
                 const unsigned int whichScene, const unsigned int whichShader, const unsigned int samples);
        virtual ~Sampler();

        virtual void renderScene (unsigned int *canvas, unsigned int tid, unsigned int numThreads) const = 0;
    };
}

#endif //MOBILERAYTRACER_SAMPLER_H