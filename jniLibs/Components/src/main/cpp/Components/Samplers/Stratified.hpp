//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_STRATIFIED_H
#define MOBILERAYTRACER_STRATIFIED_H

#include "MobileRT/Sampler.hpp"

namespace Components {
    class Stratified final : public MobileRT::Sampler {
    public:
        explicit Stratified(uint64_t domainSize,
                            unsigned int samples) noexcept;

        explicit Stratified(unsigned int width, unsigned int height,
                            unsigned int samples,
                            unsigned int blockSizeX, unsigned int blockSizeY) noexcept;

        float getSample(unsigned int sample) noexcept final;
    };
}//namespace Components

#endif //MOBILERAYTRACER_STRATIFIED_H
