//
// Created by Tiago on 09-Feb-17.
//

#ifndef MOBILERAYTRACER_RENDERER_H
#define MOBILERAYTRACER_RENDERER_H

#include <thread>
#include "Samplers/Sampler.h"
//#include <cstdlib>

namespace MobileRT {
    class Renderer {
    private:
        Sampler &samplerCamera_;
        Shader &shader_;
        const Camera &camera_;
        unsigned int width_;
        unsigned int height_;
        RGB *const accumulate_;
        unsigned int domainSize_;
        unsigned int blockSizeX_;
        unsigned int blockSizeY_;
        const unsigned int resolution_;
        RGB *const imagePlane_;
        float max_;
        Sampler &samplerPixel_;
        unsigned int sample_;
        //char padding[4] __attribute__((unused));
        std::function<float(const float)> toneMapper_;

    private:
        void renderScene(unsigned int *const bitmap, const unsigned int tid);
        void toneMapper(RGB& pixel);

    public:
        explicit Renderer(Sampler &samplerCamera, Shader &shader, const Camera &camera,
                          const unsigned int width, const unsigned int height,
                          const unsigned int blockSizeX, const unsigned int blockSizeY,
                          Sampler &samplerPixel);

        ~Renderer(void);

        void renderFrame(unsigned int *const bitmap, const unsigned int numThreads);

        void registerToneMapper(std::function<float(const float value)> toneMapper);

        void stopRender(void);

        unsigned int getSample(void);
    };
}

#endif //MOBILERAYTRACER_RENDERER_H
