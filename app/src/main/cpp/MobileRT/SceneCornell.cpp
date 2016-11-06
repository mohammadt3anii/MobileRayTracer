//
// Created by puscas on 16-10-2016.
//

#include "SceneCornell.h"
#include "Plane.h"
#include "Sphere.h"

using namespace MobileRT;

SceneCornell::SceneCornell () {
    // create one light source
    lights.push_back(new Light (RGB(1.f, 1.f, 1.f), Point(0.f, 0.99f, 0.f)));

    // white Material
    Material* lightGrayMat = new Material(RGB(.9f, .9f, .9f));
    // back wall
    primitives.push_back(new Primitive(new Plane(Point(0.f, 0.f, 1.f), Vect(0.f, 0.f, -1.f)), lightGrayMat));
    // floor
    primitives.push_back(new Primitive(new Plane(Point(0.f, -1.f, 0.f), Vect(0.f, 1.f, 0.f)), lightGrayMat));
    // ceiling
    primitives.push_back(new Primitive(new Plane(Point(0.f, 1.f, 0.f), Vect(0.f, -1.f, 0.f)), lightGrayMat));
    // red Material
    Material* redMat = new Material(RGB(0.9f, .0f, 0.f));
    // left red wall
    primitives.push_back(new Primitive(new Plane(Point(-1.f, 0.f, 0.f), Vect(1.f, 0.f, 0.f)), redMat));
    // blue Material
    Material* blueMat = new Material(RGB(0.f, .0f, 0.9f));
    // left blue wall
    primitives.push_back(new Primitive(new Plane(Point(1.f, 0.f, 0.f), Vect(-1.f, 0.f, 0.f)), blueMat));

    Material* MirrorMat = new Material(RGB(), RGB(.8f, .8f, .8f));
    // create one sphere
    primitives.push_back(new Primitive(new Sphere(new Point(.45f, -.65f, .4f), .35f), MirrorMat));
    Material* GreenMat = new Material(RGB(.0f, .9f, .0f), RGB(0.f, 0.2f, 0.f));
    primitives.push_back(new Primitive(new Sphere(new Point(-.45f, -.1f, .0f), .35f), GreenMat));
}