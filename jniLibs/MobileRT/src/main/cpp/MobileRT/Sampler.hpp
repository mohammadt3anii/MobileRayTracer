//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_SAMPLER_H
#define MOBILERAYTRACER_SAMPLER_H

#include <atomic>

namespace MobileRT {
    class Sampler {
    public:
        std::atomic<unsigned long long int> sample_;
        const unsigned long long int domainSize_;
        unsigned long long int samples_;

    public:
        explicit Sampler(unsigned long long int domainSize,
                         unsigned int samples) noexcept;

        explicit Sampler(unsigned int width, unsigned int height,
                         unsigned int samples,
                         unsigned int blockSizeX,
                         unsigned int blockSizeY) noexcept;

        virtual ~Sampler() noexcept;

        void resetSampling() noexcept;

        void stopSampling() noexcept;

        virtual float getSample(unsigned int sample) noexcept = 0;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_SAMPLER_H
