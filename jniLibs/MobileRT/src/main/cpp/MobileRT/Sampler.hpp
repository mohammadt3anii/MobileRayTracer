//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERT_SAMPLER_HPP
#define MOBILERT_SAMPLER_HPP

#include "Utils.hpp"
#include <atomic>
#include <limits>

namespace MobileRT {
  class Sampler {
    public:
			std::atomic<uint64_t> sample_ {0};
      const uint64_t domainSize_ {std::numeric_limits<uint64_t>::max()};
      unsigned int samples_ {std::numeric_limits<unsigned int>::max()};

    public:
			explicit Sampler() noexcept = default;

			explicit Sampler(unsigned int width, unsigned int height,
											 unsigned int samples,
											 unsigned int blockSizeX, unsigned int blockSizeY) noexcept;

			Sampler(const Sampler &sampler) noexcept = delete;

			Sampler(Sampler &&sampler) noexcept = delete;

			virtual ~Sampler() noexcept;

			Sampler &operator=(const Sampler &sampler) noexcept = delete;

			Sampler &operator=(Sampler &&sampler) noexcept = delete;

			void resetSampling() noexcept;

			void stopSampling() noexcept;
    virtual float getSample (unsigned int sample) noexcept = 0;
    float getSample () noexcept;
	};
}//namespace MobileRT

#endif //MOBILERT_SAMPLER_HPP
