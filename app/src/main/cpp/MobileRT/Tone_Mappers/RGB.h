//
// Created by Tiago on 16-10-2016.
//

#ifndef MOBILERAYTRACER_RGB_H
#define MOBILERAYTRACER_RGB_H

#include "ToneMapper.h"
#include <mutex>

namespace MobileRT
{
    class RGB : public ToneMapper
    {
        private:

        public:
            float R_;
            float G_;
            float B_;
        unsigned int samples_;
        std::mutex mutex;

            RGB ();
            RGB (const float r, const float g, const float b);
            RGB (const RGB& rgb);

            bool isZero() const;
            void add (const RGB& rgb);
            void mult (const RGB& rgb);
            void mult (const float f);

        void addSample(RGB &average, const RGB &sample);

        void average();

        void reset();

        void reset(const float r, const float g, const float b);

        unsigned int RGB2Color();
    };
}

#endif //MOBILERAYTRACER_RGB_H