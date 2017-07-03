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
        explicit RGB(void);

        explicit RGB(const float r, const float g, const float b);

        RGB(const RGB &rgb);

        bool isNotZero(void) const;

        void add(const RGB &rgb);

        void addMult(const RGB &rgb, const float value);

        void addMult(const RGB &rgb1, const RGB &rgb2, const float value);

        void addMult(const RGB &rgb1, const RGB &rgb2);

        const RGB operator*(const float value) const;

        void operator*=(const RGB &rgb);

        void operator*=(const float value);

        void operator/=(const float value);

        void addSampleAndCalcAvg(RGB &sample);

        void reset(void);

        void reset(const float color);

        unsigned int RGB2Color(void);

        float getMax(void);

        static unsigned int getInstances();
    };
}

#endif //MOBILERAYTRACER_RGB_H
