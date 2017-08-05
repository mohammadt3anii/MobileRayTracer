//
// Created by Tiago on 21-11-2016.
//

#include "Stratified.hpp"

using Components::Stratified;

Stratified::Stratified(const uint64_t domainSize, const unsigned int samples) noexcept
        :
        Sampler(domainSize, samples) {
}

Stratified::Stratified(const unsigned int width, const unsigned int height,
                       const unsigned int samples,
                       const unsigned int blockSizeX, const unsigned int blockSizeY) noexcept :
        Sampler(width, height, samples, blockSizeX, blockSizeY) {
}

float Stratified::getSample(const unsigned int sample) noexcept {

  const uint64_t current(this->sample_.fetch_add(1, std::memory_order_relaxed));
  if (current >= (this->domainSize_ * (sample + 1))) {
    this->sample_.fetch_sub(1, std::memory_order_relaxed);
        return 1.0f;
    }
    return static_cast<float> (current - (sample * this->domainSize_)) / this->domainSize_;
}
