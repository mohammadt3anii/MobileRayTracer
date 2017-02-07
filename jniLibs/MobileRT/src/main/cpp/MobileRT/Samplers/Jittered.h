//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_JITTERED_H
#define MOBILERAYTRACER_JITTERED_H

#include "Sampler.h"
#include <cstdlib>

namespace MobileRT {
    class Jittered : public Sampler {
    private:
        void renderScene(unsigned int *const bitmap,
                         const unsigned int threadId,
                         const unsigned int numThreads) override;

    public:
        Jittered(const unsigned int width, const unsigned int height,
                 const Shader &shader, const unsigned int samples,
                 const Camera &camera);
    };
}

#endif //MOBILERAYTRACER_JITTERED_H