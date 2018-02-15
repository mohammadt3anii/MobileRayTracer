//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_RGB_HPP
#define MOBILERT_RGB_HPP

#include <initializer_list>
#include <mutex>

namespace MobileRT {
    class RGB final {
    public:
        float R_{0.0f};
        float G_{0.0f};
        float B_{0.0f};

    private:
        void toneMap() noexcept;

    public:
        explicit RGB() noexcept = default;

        explicit RGB(float r, float g, float b) noexcept;

        RGB(const RGB &rgb) noexcept;

        RGB(RGB &&rgb) noexcept;

        ~RGB() noexcept = default;

        RGB &operator=(const RGB &rgb) noexcept;

        RGB &operator=(RGB &&rgb) noexcept;

        RGB &operator=(float value) noexcept;

        bool hasColor() const noexcept;

        void operator+=(RGB rgb) noexcept;

        void addMult(::std::initializer_list<const RGB> rgbs, float value = 1.0f) noexcept;

        RGB operator*(float value) const noexcept;

        void operator*=(RGB rgb) noexcept;

        void operator*=(float value) noexcept;

        void operator+=(float value) noexcept;

        void operator/=(float value) noexcept;

        void addSampleAndCalcAvg(RGB *sample) noexcept;

        void reset(float r = 0.0f, float g = 0.0f, float b = 0.0f) noexcept;

        unsigned RGB2Color() noexcept;

        unsigned getColor() const noexcept;

        float getMax() const noexcept;

        static unsigned getInstances() noexcept;

        static unsigned
        incrementalAvg(RGB sample, unsigned avg, unsigned numSample) noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_RGB_HPP
