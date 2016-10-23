//
// Created by puscas on 16-10-2016.
//

#include "SceneCornell.h"
#include "Plane.h"
#include "Sphere.h"

using namespace MobileRT;

SceneCornell::SceneCornell () {
    // create one light source
    lights.push_back(new Light (RGB(1.f, 1.f, 1.f), myPoint(0.f, 0.99f, 0.f)));

    // white Material
    myMaterial* lightGrayMat = new myMaterial(new RGB(.9f, .9f, .9f));
    // back wall
    primitives.push_back(new Primitive(new Plane(new myPoint(0.f, 0.f, 1.f), new myVect(0.f, 0.f, -1.f)), lightGrayMat));
    // floor
    primitives.push_back(new Primitive(new Plane(new myPoint(0.f, -1.f, 0.f), new myVect(0.f, 1.f, 0.f)), lightGrayMat));
    // ceiling
    primitives.push_back(new Primitive(new Plane(new myPoint(0.f, 1.f, 0.f), new myVect(0.f, -1.f, 0.f)), lightGrayMat));
    // red Material
    myMaterial* redMat = new myMaterial(new RGB(0.9f, .0f, 0.f));
    // left red wall
    primitives.push_back(new Primitive(new Plane(new myPoint(-1.f, 0.f, 0.f), new myVect(1.f, 0.f, 0.f)), redMat));
    // blue Material
    myMaterial* blueMat = new myMaterial(new RGB(0.f, .0f, 0.9f));
    // left blue wall
    primitives.push_back(new Primitive(new Plane(new myPoint(1.f, 0.f, 0.f), new myVect(-1.f, 0.f, 0.f)), blueMat));

    myMaterial* MirrorMat = new myMaterial(new RGB(), new RGB(.8f, .8f, .8f));
    // create one sphere
    primitives.push_back(new Primitive(new Sphere(new myPoint(.45f, -.65f, .4f), .35f), MirrorMat));
    myMaterial* GreenMat = new myMaterial(new RGB(.0f, .9f, .0f), new RGB(0.f, 0.2f, 0.f));
    primitives.push_back(new Primitive(new Sphere(new myPoint(-.45f, -.1f, .0f), .35f), GreenMat));
}