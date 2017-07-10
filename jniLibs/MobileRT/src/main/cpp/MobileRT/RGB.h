//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RGB_H
#define MOBILERAYTRACER_RGB_H

#include <mutex>

namespace MobileRT {
    class RGB final {
    private:
        std::mutex mutex_;
        unsigned int samples_;

    public:
        float R_;
        float G_;
        float B_;

    public:
        explicit RGB(void) noexcept;

        explicit RGB(const float r, const float g, const float b) noexcept;

        RGB(const RGB &rgb) noexcept;

        bool hasColor(void) const noexcept;

        void add(const RGB &rgb) noexcept;

        void addMult(const RGB &rgb, const float value) noexcept;

        void addMult(const RGB &rgb1, const RGB &rgb2, const float value) noexcept;

        void addMult(const RGB &rgb1, const RGB &rgb2) noexcept;

        const RGB operator*(const float value) const noexcept;

        void operator*=(const RGB &rgb) noexcept;

        void operator*=(const float value) noexcept;

        void operator/=(const float value) noexcept;

        void addSampleAndCalcAvg(RGB &sample) noexcept;

        void reset(void) noexcept;

        unsigned int RGB2Color(void) noexcept;

        float getMax(void) noexcept;

        static unsigned int getInstances() noexcept;
    };
}

#endif //MOBILERAYTRACER_RGB_H
