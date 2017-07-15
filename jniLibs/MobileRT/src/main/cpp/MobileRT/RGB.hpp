//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_RGB_HPP
#define MOBILERT_RGB_HPP

#include <mutex>

namespace MobileRT {
    class RGB final {
    private:
        std::mutex mutex_;
        unsigned int samples_{0u};

    public:
        float R_{0.0f};
        float G_{0.0f};
        float B_{0.0f};

    public:
        explicit RGB() noexcept;

        explicit RGB(float r, float g, float b) noexcept;

        RGB(const RGB &rgb) noexcept;

		RGB(RGB &&rgb) noexcept;

		~RGB() noexcept = default;

		RGB &operator=(const RGB &rgb) const noexcept = delete;

		RGB &&operator=(RGB &&rgb) const noexcept = delete;

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

#endif //MOBILERT_RGB_HPP
