//
// Created by puscas on 16-10-2016.
//

#include "RayTrace.hpp"
#include "ShaderNoShadows.hpp"
#include "ShaderWhitted.hpp"

using namespace MobileRT;

RayTrace::RayTrace(const Scene& scene, const unsigned int whichShader) :
    scene_(scene)
{
    switch (whichShader)
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

RayTrace::~RayTrace ()
{
    delete this->shader_;
}

void RayTrace::RayV (const Ray& ray, RGB& rgb) const//trace do raio e verifica se interseta algo
{
    // compute radiance
    Intersection intersection;
    if (this->scene_.trace(ray, intersection) == true)//se interseta alguma primitiva
    {
        this->shader_->Shade(ray, intersection, rgb);
    } else
    {
        rgb.setRGB (0.1f, 0.1f, 0.9f);//cor do pixel sem interseçao
    }
}