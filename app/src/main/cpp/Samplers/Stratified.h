//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_STRATIFIED_H
#define MOBILERAYTRACER_STRATIFIED_H

#include "../../../../../jniLibs/MobileRT/src/main/cpp/MobileRT/Samplers/Sampler.h"

namespace MobileRT {
    class Stratified : public Sampler {
    public:
        Stratified(const unsigned int domainSize, const unsigned int samples);

        float getDeviation(const unsigned int num) override;

        float getTask(const unsigned int tasks, const unsigned int sample) override;
    };
}

#endif //MOBILERAYTRACER_STRATIFIED_H
