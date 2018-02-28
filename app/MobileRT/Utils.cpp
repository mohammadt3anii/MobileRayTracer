//
// Created by Tiago on 14-11-2016.
//

#include "Utils.hpp"

namespace MobileRT {

    ::std::int32_t roundDownToMultipleOf(const ::std::int32_t value, const ::std::int32_t multiple) noexcept {
        const ::std::int32_t rest{value % multiple};
        return rest > 1 ? value - rest : value;
    }

    //https://en.wikipedia.org/wiki/Halton_sequence
    float haltonSequence(::std::uint32_t index, const ::std::uint32_t base) noexcept {
        float f{1.0f};
        float result{0.0f};
        while (index > 0) {
            f /= base;
            result += f * (index % base);
            index = static_cast<::std::uint32_t> (::std::floor(index / base));
        }
        return result;
    }

    //newAvg = ((size - 1) * oldAvg + newNum) / size;
    ::std::uint32_t incrementalAvg(const ::glm::vec3 sample, const ::std::uint32_t avg, const ::std::uint32_t numSample) noexcept {
        //toneMap();

        const ::std::uint32_t redB4{avg & 0xFF};
        const ::std::uint32_t greenB4{(avg >> 8) & 0xFF};
        const ::std::uint32_t blueB4{(avg >> 16) & 0xFF};

        const ::std::uint32_t redSamp{static_cast<::std::uint32_t> (sample[0] * 255u)};
        const ::std::uint32_t greenSamp{static_cast<::std::uint32_t> (sample[1] * 255u)};
        const ::std::uint32_t blueSamp{static_cast<::std::uint32_t> (sample[2] * 255u)};

        const ::std::uint32_t redAft{((numSample - 1) * redB4 + redSamp) / numSample};
        const ::std::uint32_t greenAft{((numSample - 1) * greenB4 + greenSamp) / numSample};
        const ::std::uint32_t blueAft{((numSample - 1) * blueB4 + blueSamp) / numSample};

        const ::std::uint32_t retR {::std::min(redAft, 255u)};
        const ::std::uint32_t retG {::std::min(greenAft, 255u)};
        const ::std::uint32_t retB {::std::min(blueAft, 255u)};

        return (0xFF000000u | (retB << 16u) | (retG << 8u) | retR);
    }
}//namespace MobileRT
