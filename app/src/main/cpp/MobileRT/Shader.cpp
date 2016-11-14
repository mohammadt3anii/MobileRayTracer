//
// Created by Tiago on 16-10-2016.
//

#include "Shader.h"

using namespace MobileRT;

Shader::Shader(RayTrace& rayTrace, const Scene& scene) :
        scene_(scene),
        rayTrace_(rayTrace)
{
}

Shader::~Shader ()
{
}

void Shader::Shade(const Ray &, Intersection &, RGB&) const
{
}