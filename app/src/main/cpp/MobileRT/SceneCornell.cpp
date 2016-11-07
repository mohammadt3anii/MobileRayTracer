//
// Created by puscas on 16-10-2016.
//

#include "SceneCornell.h"
#include "Plane.h"
#include "Sphere.h"

using namespace MobileRT;

SceneCornell::SceneCornell ()
{
    // create one light source
    lights.push_back(new Light (RGB(1.0f, 1.0f, 1.0f), Point(0.0f, 0.99f, 0.0f)));

    // white Material
    Material* lightGrayMat = new Material(RGB(0.9f, 0.9f, 0.9f));//shared
    // back wall
    primitives.push_back(new Primitive(new Plane(Point(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, -1.0f)), lightGrayMat));//recebe referencia de plano e copia
    // floor
    primitives.push_back(new Primitive(new Plane(Point(0.0f, -1.0f, 0.0f), Vect(0.0f, 1.0f, 0.0f)), lightGrayMat));
    // ceiling
    primitives.push_back(new Primitive(new Plane(Point(0.0f, 1.0f, 0.0f), Vect(0.0f, -1.0f, 0.0f)), lightGrayMat));
    // red Material
    Material* redMat = new Material(RGB(0.9f, 0.0f, 0.0f));
    // left red wall
    primitives.push_back(new Primitive(new Plane(Point(-1.0f, 0.0f, 0.0f), Vect(1.0f, 0.0f, 0.0f)), redMat));
    // blue Material
    Material* blueMat = new Material(RGB(0.0f, .0f, 0.9f));
    // left blue wall
    primitives.push_back(new Primitive(new Plane(Point(1.0f, 0.0f, 0.0f), Vect(-1.0f, 0.0f, 0.0f)), blueMat));

    Material* MirrorMat = new Material(RGB(), RGB(0.8f, 0.8f, 0.8f));
    // create one sphere
    primitives.push_back(new Primitive(new Sphere(new Point(0.45f, -0.65f, 0.4f), 0.35f), MirrorMat));
    Material* GreenMat = new Material(RGB(.0f, .9f, .0f), RGB(0.0f, 0.2f, 0.0f));
    primitives.push_back(new Primitive(new Sphere(new Point(-0.45f, -0.1f, 0.0f), 0.35f), GreenMat));
}