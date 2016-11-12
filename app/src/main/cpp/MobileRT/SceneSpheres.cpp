//
// Created by puscas on 16-10-2016.
//

#include "SceneSpheres.h"
#include "Sphere.h"
#include "Plane.h"

using namespace MobileRT;

SceneSpheres::SceneSpheres ()
{
    // create one light source
    this->lights.push_back(new Light (RGB(1.0f, 1.0f, 1.0f), Point(0.0f, 15.0f, 4.0f)));

    // create diffuse Materials
    Material sandMat (RGB(0.914f, 0.723f, 0.531f));
    Material redMat (RGB(0.9f, 0.0f, 0.0f));
    Material mirrorMat (RGB(), RGB(0.8f, 0.8f, 0.8f));
    Material greenMat (RGB(0.0f, 0.9f, 0.0f));
    // create one sphere
    this->primitives.push_back(new Primitive(new Sphere(Point(-1.0f, 1.0f, 6.0f), 1.0f), redMat));
    this->primitives.push_back(new Primitive(new Sphere(Point(1.5f, 2.0f, 7.0f), 1.0f), mirrorMat));
    this->primitives.push_back(new Primitive(new Plane(Point(0.0f, 0.0f, 0.0f), Vect(0.0f, 1.0f, 0.0f)), sandMat));
    this->primitives.push_back(new Primitive(new Sphere(Point(0.0f, 0.5f, 4.5f), 0.5f), greenMat));
}