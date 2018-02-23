//
// Created by Tiago on 14-11-2016.
//

#include "Utils.hpp"

int ::MobileRT::roundDownToMultipleOf(const int value, const int multiple) noexcept {
    const int rest{value % multiple};
    return rest > 1 ? value - rest : value;
}

//https://en.wikipedia.org/wiki/Halton_sequence
float ::MobileRT::haltonSequence(uint32_t index, const unsigned base) noexcept {
    float f{1.0f};
    float result{0.0f};
    while (index > 0) {
        f /= base;
        result += f * (index % base);
        index = static_cast<uint32_t> (::std::floor(index / base));
    }
    return result;
}

//newAvg = ((size - 1) * oldAvg + newNum) / size;
unsigned ::MobileRT::incrementalAvg(const glm::vec3 sample, const unsigned avg, const unsigned numSample) noexcept {
    //toneMap();

    const unsigned redB4{avg & 0xFF};
    const unsigned greenB4{(avg >> 8) & 0xFF};
    const unsigned blueB4{(avg >> 16) & 0xFF};

    const unsigned redSamp{static_cast<unsigned> (sample.r * 255u)};
    const unsigned greenSamp{static_cast<unsigned> (sample.g * 255u)};
    const unsigned blueSamp{static_cast<unsigned> (sample.b * 255u)};

    const unsigned redAft{((numSample - 1) * redB4 + redSamp) / numSample};
    const unsigned greenAft{((numSample - 1) * greenB4 + greenSamp) / numSample};
    const unsigned blueAft{((numSample - 1) * blueB4 + blueSamp) / numSample};

    unsigned retR{redAft > 255u ? 255u : redAft};
    unsigned retG{greenAft > 255u ? 255u : greenAft};
    unsigned retB{blueAft > 255u ? 255u : blueAft};

    return (0xFF000000u | (retB << 16u) | (retG << 8u) | retR);
}
