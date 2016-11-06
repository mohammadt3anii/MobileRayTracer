//
// Created by puscas on 16-10-2016.
//

#include "SceneSpheres.h"
#include "Sphere.h"
#include "Plane.h"

using namespace MobileRT;

SceneSpheres::SceneSpheres () {
    // create one light source
    lights.push_back(new Light (RGB(1.f, 1.f, 1.f), Point(0.f, 15.f, 4.f)));

    // create diffuse Materials
    Material* sandMat = new Material(RGB(.914f, .723f, .531f));
    Material* redMat = new Material(RGB(0.9f, .0f, 0.f));
    Material* mirrorMat = new Material(RGB(), RGB(.8f, .8f, .8f));
    Material* greenMat = new Material(RGB(.0f, .9f, .0f));
    // create one sphere
    primitives.push_back(new Primitive(new Sphere(new Point(-1.f, 1.f, 6.f)), redMat));
    primitives.push_back(new Primitive(new Sphere(new Point(1.5f, 2.f, 7.f)), mirrorMat));
    primitives.push_back(new Primitive(new Plane(Point(0.f, 0.f, 0.f), Vect(0.f, 1.f, 0.f)), sandMat));
    primitives.push_back(new Primitive(new Sphere(new Point(.0f, .5f, 4.5f), .5f), greenMat));
}