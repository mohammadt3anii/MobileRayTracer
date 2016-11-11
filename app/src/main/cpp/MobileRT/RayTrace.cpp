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

RGB RayTrace::RayV (const Ray& ray)//trace do raio e verifica se interseta algo
{
    // compute radiance
    const Intersection* intersection = this->scene_.trace(ray);//trace do raio
    if (intersection != nullptr)//se interseta alguma primitiva
    {
        return this->shader_->Shade(ray, *intersection);
    }
    return RGB(0.1f, 0.1f, 0.9f);//cor do pixel sem interseçao
}