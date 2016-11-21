//
// Created by Tiago on 21-11-2016.
//

#include "Jittered.h"
#include "../Utils.h"
#include "../Color_Models/ToneMapper.h"
#include <cstdlib>

using namespace MobileRT;

Jittered::Jittered(const unsigned int width, const unsigned int height,
            const unsigned int whichScene, const unsigned int whichShader, const unsigned int samples) :
    Sampler(width, height, whichScene, whichShader, samples)
{
}

void Jittered::renderScene(unsigned int *canvas, unsigned int tid,
                                 unsigned int numThreads) const {
    const float width (this->width_);
    const float height (this->height_);
    const float INV_IMG_WIDTH (1.0f / width);
    const float INV_IMG_HEIGHT (1.0f / height);
    const float jitter_max (0.001f);
    const float half_rand_max(static_cast<float>(RAND_MAX/2));
    RGB rayRGB;
    RGB rgb;
    Intersection isect;
    Vector3D vector;
    Ray ray;
    for (unsigned int y (tid); y < height; y += numThreads)
    {
        const unsigned int yWidth(y * this->width_);
        const float v (static_cast<float>(y * INV_IMG_HEIGHT));
        const float v_alpha (fastArcTan(this->camera_->vFov_ * (0.5f - v)));
        for (unsigned int x (0); x < width; x += 1)
        {
            // generate the ray
            const float u (static_cast<float>(x * INV_IMG_WIDTH));
            const float u_alpha (fastArcTan(this->camera_->hFov_ * (u - 0.5f)));
            float count = 0.0f;
            rgb.recycle();
            for(unsigned int i (0); i < this->samples_; i++)
            {
                const float randU(std::rand() / half_rand_max - 1.0f);
                const float randV(std::rand() / half_rand_max - 1.0f);
                const float u_alpha_jittered (u_alpha + (randU * jitter_max));
                const float v_alpha_jittered (v_alpha + (randV * jitter_max));
                this->camera_->getRay(ray, u_alpha_jittered,v_alpha_jittered);//constroi raio e coloca em ray
                this->rayTracer_->rayTrace(rayRGB, ray, isect, vector);//faz trace do raio e coloca a cor em rayRGB
                rgb.add(rayRGB);
                count += 1.0f;
            }
            rgb.mult(1.0f / count);
            canvas[x + yWidth] = ToneMapper::RGB2Color(rgb);// tonemap and convert to Paint
        }
    }
}