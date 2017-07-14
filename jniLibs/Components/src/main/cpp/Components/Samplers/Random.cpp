//
// Created by Tiago on 19-04-2017.
//

#include "Random.hpp"

using Components::Random;

Random::Random(const unsigned long long int domainSize, const unsigned int samples) noexcept :
        Sampler(domainSize, samples) {
}

float Random::getSample(const unsigned int) noexcept {
    thread_local static std::uniform_real_distribution<float> uniform_dist(0.0f, 1.0f);
    thread_local static std::mt19937 gen(std::random_device{}());
	//thread_local static std::mt19937 gen(0);
    return uniform_dist(gen);

    //return static_cast<float> (rand()) / RAND_MAX;
}
