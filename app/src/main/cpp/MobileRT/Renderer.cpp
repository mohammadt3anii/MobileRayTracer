//
// Created by puscas on 16-10-2016.
//

#include "Renderer.h"
#include "SceneCornell.h"
#include "SceneSpheres.h"
#include "ToneMapper.h"

using namespace MobileRT;

Renderer::Renderer(const int pcanvasW, const int pcanvasH, const int renderRes, const int whichScene, const int whichShader) {
    RT_H = RT_W = renderRes;
    LowX = (pcanvasW-RT_W) >> 1;
    LowY = (pcanvasH-RT_H) >> 1;

    float ratio = static_cast<float>(RT_H) / static_cast<float>(RT_W);

    // create and load the Scene, parameterize the camera
    switch (whichScene) {
        case 0 : // cornell
        {
            scene_ = std::unique_ptr<SceneCornell>(new SceneCornell());
            float hFov = 45.0f;
            float vFov = hFov * ratio;
            camera_ = std::unique_ptr<RTCamera>(new RTCamera(Point(0.0f, 0.0f, -3.4f), hFov, vFov));
        }
            break;
        case 1 : // spheres
        {
            scene_ = std::unique_ptr<SceneSpheres>(new SceneSpheres());
            float hFov = 60.0f;
            float vFov = hFov * ratio;
            camera_ = std::unique_ptr<RTCamera>(new RTCamera(Point(0.0f, 0.5f, 1.0f), hFov, vFov));
        }
            break;
        default:
            break;
    }

    // create the ray tracer
    rTracer_ = std::unique_ptr<RayTrace>(new RayTrace(*scene_, whichShader));
}

void Renderer::render(uint32_t *canvas, const int width) {//TODO: permitir lançar mais de 1 raio por pixel
    const float INV_IMG_WIDTH = 1.0f / RT_W;
    const float INV_IMG_HEIGHT = 1.0f / RT_H;
    for(int y = 0; y < RT_H; y++) {
        for (int x = 0; x < RT_W; x++) {
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