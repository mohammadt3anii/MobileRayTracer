//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RGB_H
#define MOBILERAYTRACER_RGB_H

#include <mutex>

namespace MobileRT {
    class RGB {
    public:
        float R_;
        float G_;
        float B_;
        std::mutex mutex_;
        unsigned int samples_;
        char padding[4] __attribute__((unused));

    public:
        explicit RGB(void);

        explicit RGB(const float r, const float g, const float b);

        explicit RGB(const RGB &rgb);

        bool isZero(void) const;
        void add(const RGB &rgb);
        void mult(const RGB &rgb);
        void mult(const float f);
        void addSample(RGB &average, const RGB &sample);

        void average(void);

        void reset(void);

        void reset(const float color);
        void reset(const float r, const float g, const float b);

        unsigned int RGB2Color(void);

        static unsigned int getInstances();
    };
}

#endif //MOBILERAYTRACER_RGB_H
