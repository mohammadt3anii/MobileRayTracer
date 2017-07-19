//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERT_SAMPLER_HPP
#define MOBILERT_SAMPLER_HPP

#include <atomic>

namespace MobileRT {
    class Sampler {
    public:
        std::atomic<uint64_t> sample_;
        const uint64_t domainSize_;
        uint64_t samples_;

    public:
        explicit Sampler(uint64_t domainSize,
                         unsigned int samples) noexcept;

        explicit Sampler(unsigned int width, unsigned int height,
                         unsigned int samples,
                         unsigned int blockSizeX,
                         unsigned int blockSizeY) noexcept;

		Sampler(const Sampler &sampler) noexcept = delete;

		Sampler(Sampler &&sampler) noexcept = delete;

        virtual ~Sampler() noexcept = default;

		Sampler &operator=(const Sampler &sampler) noexcept = delete;

		Sampler &operator=(Sampler &&sampler) noexcept = delete;

        void resetSampling() noexcept;

        void stopSampling() noexcept;

        virtual float getSample(unsigned int sample) noexcept = 0;
    };
}//namespace MobileRT

#endif //MOBILERT_SAMPLER_HPP
