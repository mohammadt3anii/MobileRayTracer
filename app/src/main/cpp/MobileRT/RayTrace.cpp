//
// Created by puscas on 16-10-2016.
//

#include "RayTrace.h"
#include "ShaderNoShadows.h"
#include "ShaderWhitted.h"

using namespace MobileRT;

RayTrace::RayTrace(Scene &scene, const int &whichShader) :
    scene_(scene)
{
    switch (whichShader)
    {
        case 0:
        {
            this->shader_ = std::unique_ptr<ShaderNoShadows>(new ShaderNoShadows(*this, scene));
        }
            break;
        case 1:
        {
            this->shader_ = std::unique_ptr<ShaderWhitted>(new ShaderWhitted(*this, scene));
        }
            break;
    }
}

RGB RayTrace::RayV(const Ray &ray)//trace do raio e verifica se interseta algo
{
    // compute radiance
    const Intersection intersection(this->scene_.trace(ray));//trace do raio
    if (intersection.intersected())//se interseta alguma primitiva
    {
        return this->shader_->Shade(ray, intersection);
    }
    return RGB(0.1f, 0.1f, 0.9f);//cor do pixel sem interseçao
}