//
// Created by Tiago on 16-10-2016.
//

#include "RayTracer.h"
#include "Shaders/ShaderNoShadows.h"
#include "Shaders/ShaderWhitted.h"

using namespace MobileRT;

RayTracer::RayTracer(const Scene& scene, const unsigned int shader) :
    scene_(scene)
{
    switch (shader)
    {
        case 0:
        {
            this->shader_ = new ShaderNoShadows(*this, scene);
        }
            break;

        case 1:
        {
            this->shader_ = new ShaderWhitted(*this, scene);
        }
            break;

        default:
            break;
    }
}

RayTracer::~RayTracer ()
{
    delete this->shader_;
}

void RayTracer::rayTrace (Ray& ray, RGB& rgb, Intersection& intersection) const//trace do raio e verifica se interseta algo
{
    // compute radiance
    if (this->scene_.trace(ray, intersection) == true)//se interseta alguma primitiva
    {
        this->shader_->shade(ray, intersection, rgb);
    }
    else
    {
        rgb.setRGB (0.1f, 0.1f, 0.9f);//cor do pixel sem interseçao
    }
}