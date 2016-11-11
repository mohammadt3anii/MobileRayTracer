//
// Created by puscas on 16-10-2016.
//

#include "SceneCornell.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"

using namespace MobileRT;

SceneCornell::SceneCornell ()
{
    // create one light source
    this->lights.push_back( new Light (RGB(1.0f, 1.0f, 1.0f), Point(0.0f, 0.99f, 0.0f)));

    // white Material
    const Material lightGrayMat (RGB(0.9f, 0.9f, 0.9f));

    // back wall
    this->primitives.push_back( new Primitive( new Plane(Point(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, -1.0f)), lightGrayMat));
    // floor
    
    this->primitives.push_back( new Primitive( new Plane(Point(0.0f, -1.0f, 0.0f), Vect(0.0f, 1.0f, 0.0f)), lightGrayMat));
    // ceiling
    this->primitives.push_back( new Primitive( new Plane(Point(0.0f, 1.0f, 0.0f), Vect(0.0f, -1.0f, 0.0f)), lightGrayMat));
    // red Material
    const Material redMat (RGB(0.9f, 0.0f, 0.0f));
    // left red wall
    this->primitives.push_back( new Primitive( new Plane(Point(-1.0f, 0.0f, 0.0f), Vect(1.0f, 0.0f, 0.0f)), redMat));
    // blue Material
    const Material blueMat (RGB(0.0f, 0.0f, 0.9f));
    // left blue wall
    this->primitives.push_back( new Primitive( new Plane(Point(1.0f, 0.0f, 0.0f), Vect(-1.0f, 0.0f, 0.0f)), blueMat));

    // create one sphere
    const Material MirrorMat (RGB(), RGB(0.8f, 0.8f, 0.8f));
    this->primitives.push_back( new Primitive( new Sphere(Point(0.45f, -0.65f, 0.4f), 0.35f), MirrorMat));

    const Material GreenMat (RGB(0.0f, 0.9f, 0.0f), RGB(0.0f, 0.2f, 0.0f));
    this->primitives.push_back( new Primitive( new Sphere(Point(-0.45f, -0.1f, 0.0f), 0.35f), GreenMat));
}