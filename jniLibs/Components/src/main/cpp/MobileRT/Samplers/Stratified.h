//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_STRATIFIED_H
#define MOBILERAYTRACER_STRATIFIED_H

#include "../../../../../../MobileRT/src/main/cpp/MobileRT/Samplers/Sampler.h"

namespace MobileRT {
    class Stratified : public Sampler {
    public:
        explicit Stratified(const unsigned long long int domainSize, const unsigned int samples);

        explicit Stratified(const unsigned int width, const unsigned int height,
                            const unsigned int samples,
                            const unsigned int blockSizeX, const unsigned int blockSizeY);

        float getSample(const unsigned int sample) override;
    };
}

#endif //MOBILERAYTRACER_STRATIFIED_H
