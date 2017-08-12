//
// Created by Tiago on 21-11-2016.
//

#include "Sampler.hpp"

using MobileRT::Sampler;

Sampler::Sampler (const unsigned int width, const unsigned int height,
                  const unsigned int samples) noexcept :
  domainSize_ {(width / (width / static_cast<unsigned int>(std::sqrt (NumberOfBlocks)))) *
               (height / (width / static_cast<unsigned int>(std::sqrt (NumberOfBlocks))))},
  samples_ {samples}
{
}

void Sampler::resetSampling() noexcept {
  this->sample_ = 0;
}

void Sampler::stopSampling() noexcept {
    this->samples_ = 0;
}

Sampler::~Sampler () noexcept {
	LOG("SAMPLER DELETED");
}

float Sampler::getSample () noexcept {
  return getSample (0);
}
