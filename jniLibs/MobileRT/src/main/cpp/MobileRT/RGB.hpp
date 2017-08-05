//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERT_RGB_HPP
#define MOBILERT_RGB_HPP

#include <mutex>

namespace MobileRT {
    class RGB final {
    public:
        float R_{0.0f};
        float G_{0.0f};
        float B_{0.0f};

	private:
		std::mutex mutex_;

	public:
		unsigned int samples_;

	private:
		void toneMap () noexcept;

    public:
		explicit RGB(float r = 0.0f, float g = 0.0f, float b = 0.0f) noexcept;

		RGB(const RGB &rgb) noexcept;

		RGB(RGB &&rgb) noexcept;

		~RGB() noexcept = default;

		RGB &operator=(const RGB &rgb) noexcept = delete;

		RGB &operator=(RGB &&rgb) noexcept = delete;

		bool hasColor() const noexcept;

		void operator+=(const RGB &rgb) noexcept;

		void addMult(const RGB &rgb, float value) noexcept;

		void addMult(const RGB &rgb1, const RGB &rgb2, float value) noexcept;

		void addMult(const RGB &rgb1, const RGB &rgb2) noexcept;

		RGB operator*(float value) const noexcept;

		void operator*=(const RGB &rgb) noexcept;

		void operator*=(float value) noexcept;

		void operator+=(float value) noexcept;

		void operator/=(float value) noexcept;

		void addSampleAndCalcAvg(RGB *sample) noexcept;

			void reset(float r = 0.0f, float g = 0.0f, float b = 0.0f, unsigned int samples = 0) noexcept;

		unsigned int RGB2Color() noexcept;

		unsigned int getColor() noexcept;

		float getMax() noexcept;

		static unsigned int getInstances() noexcept;
    };
}//namespace MobileRT

#endif //MOBILERT_RGB_HPP
