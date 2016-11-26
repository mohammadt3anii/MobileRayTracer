//
// Created by Tiago on 21-11-2016.
//

#include "Stratified.h"
#include "../Utils.h"
#include "../Color_Models/ToneMapper.h"
#include <cmath>

using namespace MobileRT;

Stratified::Stratified(const unsigned int width, const unsigned int height,
            const unsigned int whichScene, const unsigned int whichShader, const unsigned int samples) :
    Sampler(width, height, whichScene, whichShader, std::sqrt(samples)) {
}

void Stratified::renderScene(unsigned int *canvas, unsigned int tid,
                                 unsigned int numThreads) const {
    const float width (this->width_);
    const float height (this->height_);
    const float INV_IMG_WIDTH (1.0f / width);
    const float INV_IMG_HEIGHT (1.0f / height);
    RGB rayRGB;
    Intersection isect;
    Vector3D vector;
    Ray ray;
    if (this->samples_ < 2)//1 sample per pixel
    {
        for (unsigned int y (tid); y < height; y += numThreads)
        {
            const unsigned int yWidth(y * this->width_);
            const float v (static_cast<float>(y * INV_IMG_HEIGHT));
            const float v_alpha (fastArcTan(this->camera_->vFov_ * (0.5f - v)));
            for (unsigned int x (0); x < width; x += 1)
            {
                const float u (static_cast<float>(x * INV_IMG_WIDTH));
                const float u_alpha (fastArcTan(this->camera_->hFov_ * (u - 0.5f)));
                for(unsigned int i (0); i < this->samples_; i++)
                {
                    for(unsigned int j (0); j < this->samples_; j++)
                    {
                        this->camera_->getRay(ray, u_alpha, v_alpha);//constroi raio e coloca em ray
                        this->rayTracer_->rayTrace(rayRGB, ray, isect, vector);//faz trace do raio e coloca a cor em rayRGB
                    }
                }
                canvas[x + yWidth] = ToneMapper::RGB2Color(rayRGB);
            }
        }
    }
    else//multiple samples per pixel
    {
        RGB rgb;
        const float pixelHeight (0.5f/this->height_);
        const float pixelWidth (0.5f/this->width_);
        for (unsigned int y (tid); y < height; y += numThreads)
        {
            const unsigned int yWidth(y * this->width_);
            const float v (static_cast<float>(y * INV_IMG_HEIGHT));
            const float v_alpha (fastArcTan(this->camera_->vFov_ * (0.5f - v)));
            for (unsigned int x (0); x < width; x += 1)
            {
                const float u (static_cast<float>(x * INV_IMG_WIDTH));
                const float u_alpha (fastArcTan(this->camera_->hFov_ * (u - 0.5f)));
                float count (0.0f);
                rgb.recycle();
                for(unsigned int i (1); i <= this->samples_; i++)
                {
                    const float stratV (deviation(i));
                    for(unsigned int j (1); j <= this->samples_; j++)
                    {
                        const float stratU (deviation(j));
                        const float u_alpha_stratified (u_alpha + (stratU * pixelWidth));
                        const float v_alpha_stratified (v_alpha + (stratV * pixelHeight));
                        this->camera_->getRay(ray, u_alpha_stratified, v_alpha_stratified);//constroi raio e coloca em ray
                        this->rayTracer_->rayTrace(rayRGB, ray, isect, vector);//faz trace do raio e coloca a cor em rayRGB
                        rgb.add(rayRGB);
                        count += 1.0f;
                    }
                }
                rgb.mult(1.0f / count);
                canvas[x + yWidth] = ToneMapper::RGB2Color(rgb);
            }
        }
    }
}