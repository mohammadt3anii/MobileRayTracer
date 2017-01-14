//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_JITTERED_H
#define MOBILERAYTRACER_JITTERED_H

#include "Sampler.h"

namespace MobileRT {
    class Jittered : public Sampler {
    private:

    public:
        Jittered(const unsigned int width, const unsigned int height,
                 const unsigned int whichShader, const unsigned int samples,
                 const Perspective &camera, const Scene &scene);

        void renderScene(unsigned int *canvas,
                         const unsigned int threadId,
                         const unsigned int numThreads) override;
    };
}

#endif //MOBILERAYTRACER_JITTERED_H