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
        explicit Sampler(const unsigned long long int domainSize,
                         const unsigned int samples) noexcept;

        explicit Sampler(const unsigned int width, const unsigned int height,
                         const unsigned int samples,
                         const unsigned int blockSizeX,
                         const unsigned int blockSizeY) noexcept;

        virtual ~Sampler(void) noexcept;

        void resetSampling(void) noexcept;

        void stopSampling(void) noexcept;

        virtual float getSample(const unsigned int sample) noexcept = 0;
    };
}

#endif //MOBILERAYTRACER_SAMPLER_H
