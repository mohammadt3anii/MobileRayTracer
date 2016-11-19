//
// Created by Tiago on 16-10-2016.
//

#include "RayTracer.h"
#include "Shaders/ShaderNoShadows.h"
#include "Shaders/ShaderWhitted.h"

using namespace MobileRT;

RayTracer::RayTracer(const Scene &scene, const unsigned int shader) :
        scene_(scene) {
    switch (shader) {
        case 0: {
            this->shader_ = new ShaderNoShadows(*this, scene);
        }
            break;

        case 1: {
            this->shader_ = new ShaderWhitted(*this, scene);
        }
            break;

        default:
            break;
    }
}

RayTracer::~RayTracer() {
    delete this->shader_;
}

//trace do raio e verifica se interseta algo
void RayTracer::rayTrace(RGB &rgb, Ray &ray, Intersection &intersection, Vector3D &vector) const {
    // compute radiance
    if (this->scene_.trace(intersection, ray))//se interseta alguma primitiva
    {
        this->shader_->shade(rgb, intersection, ray, vector);
    }
    else {
        rgb.recycle(0.1f, 0.1f, 0.9f);//cor do pixel sem interseçao
    }
}