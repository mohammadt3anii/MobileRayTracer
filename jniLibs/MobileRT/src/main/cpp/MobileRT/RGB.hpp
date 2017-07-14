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
        explicit RGB() noexcept;

        explicit RGB(float r, float g, float b) noexcept;

        RGB(const RGB &rgb) noexcept;

        bool hasColor() const noexcept;

        void add(const RGB &rgb) noexcept;

        void addMult(const RGB &rgb, float value) noexcept;

        void addMult(const RGB &rgb1, const RGB &rgb2, float value) noexcept;

        void addMult(const RGB &rgb1, const RGB &rgb2) noexcept;

        const RGB operator*(float value) const noexcept;

        void operator*=(const RGB &rgb) noexcept;

        void operator*=(float value) noexcept;

        void operator/=(float value) noexcept;

        void addSampleAndCalcAvg(RGB &sample) noexcept;

        void reset() noexcept;

        unsigned int RGB2Color() noexcept;

        float getMax() noexcept;

        static unsigned int getInstances() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERAYTRACER_RGB_H
