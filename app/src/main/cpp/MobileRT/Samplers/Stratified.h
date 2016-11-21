//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_STRATIFIED_H
#define MOBILERAYTRACER_STRATIFIED_H

#include "Sampler.h"

namespace MobileRT {
    class Stratified : public Sampler {
    private:
        

    public:
        Stratified(const unsigned int width, const unsigned int height,
                 const unsigned int whichScene, const unsigned int whichShader, const unsigned int samples);
        void renderScene (unsigned int *canvas, unsigned int tid, unsigned int numThreads) const override;
    };
}

#endif //MOBILERAYTRACER_STRATIFIED_H