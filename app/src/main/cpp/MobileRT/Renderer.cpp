//
// Created by puscas on 16-10-2016.
//

#include "Renderer.h"
#include "SceneCornell.h"
#include "SceneSpheres.h"
#include "ToneMapper.h"

using namespace MobileRT;

Renderer::Renderer(const int pcanvasW, const int pcanvasH, const int renderRes, const int whichScene, const int whichShader) :
    RT_W(renderRes),
    RT_H(renderRes),
    LowX((pcanvasW-RT_W) >> 1),
    LowY((pcanvasH-RT_H) >> 1)
{
    const float ratio = static_cast<float>(RT_H) / static_cast<float>(RT_W);

    // create and load the Scene, parameterize the camera
    switch (whichScene)
    {
        case 0 : // cornell
        {
            this->scene_ = std::unique_ptr<SceneCornell>(new SceneCornell());
            const float hFov = 45.0f;
            const float vFov = hFov * ratio;
            this->camera_ = std::unique_ptr<RTCamera>(new RTCamera(Point(0.0f, 0.0f, -3.4f), hFov, vFov));
        }
            break;

        case 1 : // spheres
        {
            this->scene_ = std::unique_ptr<SceneSpheres>(new SceneSpheres());
            const float hFov = 60.0f;
            const float vFov = hFov * ratio;
            this->camera_ = std::unique_ptr<RTCamera>(new RTCamera(Point(0.0f, 0.5f, 1.0f), hFov, vFov));
        }
            break;

        default:
            break;
    }

    // create the ray tracer
    this->rTracer_ = std::unique_ptr<RayTrace>(new RayTrace(*scene_, whichShader));
}

void Renderer::render(uint32_t *canvas, const int width) const//TODO: permitir lançar mais de 1 raio por pixel
{
    const float INV_IMG_WIDTH = 1.0f / this->RT_W;
    const float INV_IMG_HEIGHT = 1.0f / this->RT_H;
    //#pragma omp parallel for num_threads(4)
    for(int y = 0; y < this->RT_H; y++)
    {
        for (int x = 0; x < this->RT_W; x++)
        {
            // generate the ray
            const float u = static_cast<float>(x * INV_IMG_WIDTH);
            const float v = static_cast<float>(y * INV_IMG_HEIGHT);
            const Ray r = camera_->getRay(u, v);//constroi raio
            const RGB rayRGB(rTracer_->RayV(r));//faz trace do raio

            // tonemap and convert to Paint
            canvas[x + y * width] = ToneMapper::RGB2Color(rayRGB);
        }
    }
}