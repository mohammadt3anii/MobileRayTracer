//
// Created by Tiago on 21-11-2016.
//

#ifndef MOBILERAYTRACER_STRATIFIED_H
#define MOBILERAYTRACER_STRATIFIED_H

#include "MobileRT/Sampler.h"

namespace Components {
    using namespace MobileRT;

    class Stratified final : public Sampler {
		public:
			explicit Stratified(const unsigned long long int domainSize, const unsigned int samples);

			explicit Stratified(const unsigned int width, const unsigned int height,
								const unsigned int samples,
								const unsigned int blockSizeX, const unsigned int blockSizeY);

			virtual float getSample(const unsigned int sample) override final;
    };
}

#endif //MOBILERAYTRACER_STRATIFIED_H
