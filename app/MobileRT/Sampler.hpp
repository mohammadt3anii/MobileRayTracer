//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERT_SAMPLER_HPP
#define MOBILERT_SAMPLER_HPP

#include "MobileRT/Utils.hpp"
#include <atomic>
#include <limits>

namespace MobileRT {
    class Sampler {
    public:
        ::std::atomic<::std::uint32_t> sample_{0};
        const ::std::uint32_t domainSize_{::std::numeric_limits<::std::uint32_t>::max()};
        ::std::uint32_t samples_{::std::numeric_limits<::std::uint32_t>::max()};

    public:
        explicit Sampler() noexcept = default;

        explicit Sampler(::std::uint32_t width, ::std::uint32_t height,
                         ::std::uint32_t samples) noexcept;

        Sampler(const Sampler &sampler) noexcept = delete;

        Sampler(Sampler &&sampler) noexcept = delete;

        virtual ~Sampler() noexcept;

        Sampler &operator=(const Sampler &sampler) noexcept = delete;

        Sampler &operator=(Sampler &&sampler) noexcept = delete;

        void resetSampling() noexcept;

        void stopSampling() noexcept;

        virtual float getSample(::std::uint32_t sample) noexcept = 0;

        float getSample() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_SAMPLER_HPP
