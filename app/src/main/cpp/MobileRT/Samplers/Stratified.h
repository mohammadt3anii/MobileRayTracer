//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_STRATIFIED_H
#define MOBILERAYTRACER_STRATIFIED_H

#include "Sampler.h"

namespace MobileRT {
    class Stratified : public Sampler {
    private:
        void renderScene(unsigned int *const bitmap,
                         const unsigned int threadId,
                         const unsigned int numThreads) override;

    public:
        Stratified(const unsigned int width, const unsigned int height,
                   const Shader &shader, const unsigned int samples,
                   const Camera &camera);
    };
}

#endif //MOBILERAYTRACER_STRATIFIED_H