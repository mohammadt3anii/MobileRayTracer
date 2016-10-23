//
// Created by puscas on 16-10-2016.
//

#include "SceneSpheres.h"
#include "Sphere.h"
#include "Plane.h"

using namespace MobileRT;

SceneSpheres::SceneSpheres () {
    // create one light source
    lights.push_back(new Light (RGB(1.f, 1.f, 1.f), myPoint(0.f, 15.f, 4.f)));

    // create diffuse Materials
    myMaterial* sandMat = new myMaterial(new RGB(.914f, .723f, .531f));
    myMaterial* redMat = new myMaterial(new RGB(0.9f, .0f, 0.f));
    myMaterial* MirrorMat = new myMaterial(new RGB(), new RGB(.8f, .8f, .8f));
    myMaterial* GreenMat = new myMaterial(new RGB(.0f, .9f, .0f));
    // create one sphere
    primitives.push_back(new Primitive(new Sphere(new myPoint(-1.f, 1.f, 6.f)), redMat));
    primitives.push_back(new Primitive(new Sphere(new myPoint(1.5f, 2.f, 7.f)), MirrorMat));
    primitives.push_back(new Primitive(new Plane(new myPoint(0.f, 0.f, 0.f), new myVect(0.f, 1.f, 0.f)), sandMat));
    primitives.push_back(new Primitive(new Sphere(new myPoint(.0f, .5f, 4.5f), .5f), GreenMat));
}