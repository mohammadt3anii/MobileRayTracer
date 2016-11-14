//
// Created by Tiago on 16-10-2016.
//

#include "Shader.h"

using namespace MobileRT;

Shader::Shader(RayTracer& rayTracer, const Scene& scene) :
        scene_(scene),
        rayTracer_(rayTracer)
{
}

Shader::~Shader ()
{
}

void Shader::shade(const Ray &, Intersection &, RGB&) const
{
}