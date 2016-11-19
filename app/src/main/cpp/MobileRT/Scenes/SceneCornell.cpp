//
// Created by Tiago on 16-10-2016.
//

#include "SceneCornell.h"
#include "../Shapes/Plane.h"
#include "../Shapes/Sphere.h"
#include "../Shapes/Triangle.h"

using namespace MobileRT;

SceneCornell::SceneCornell() {
    // point light - white
    this->lights.push_back(new PointLight(RGB(1.0f, 1.0f, 1.0f), Point3D(0.0f, 0.99f, 0.0f)));

    // back wall - white
    const Material lightGrayMat(RGB(0.9f, 0.9f, 0.9f));
    this->primitives.push_back(new Primitive(new Plane(
            Point3D(0.0f, 0.0f, 1.0f), Vector3D(0.0f, 0.0f, -1.0f)), lightGrayMat));
    // floor - white
    this->primitives.push_back(new Primitive(new Plane(
            Point3D(0.0f, -1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f)), lightGrayMat));
    // ceiling - white
    this->primitives.push_back(new Primitive(new Plane(
            Point3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, -1.0f, 0.0f)), lightGrayMat));
    // left wall - red
    const Material redMat(RGB(0.9f, 0.0f, 0.0f));
    this->primitives.push_back(new Primitive(new Plane(
            Point3D(-1.0f, 0.0f, 0.0f), Vector3D(1.0f, 0.0f, 0.0f)), redMat));
    // right wall - blue
    const Material blueMat(RGB(0.0f, 0.0f, 0.9f));
    this->primitives.push_back(new Primitive(new Plane(
            Point3D(1.0f, 0.0f, 0.0f), Vector3D(-1.0f, 0.0f, 0.0f)), blueMat));

    // sphere - mirror
    const Material MirrorMat(RGB(0.0f, 0.0f, 0.0f), RGB(0.8f, 0.8f, 0.8f));
    this->primitives.push_back(new Primitive(new Sphere(
            Point3D(0.45f, -0.65f, 0.4f), 0.35f), MirrorMat));

    // sphere - green
    const Material GreenMat(RGB(0.0f, 0.9f, 0.0f), RGB(0.0f, 0.2f, 0.0f));
    this->primitives.push_back(new Primitive(new Sphere(
            Point3D(-0.45f, -0.1f, 0.0f), 0.35f), GreenMat));

    // triangle - yellow
    const Material yellow(RGB(1.0f, 1.0f, 0.0f));
    this->primitives.push_back(new Primitive(new Triangle(
            Point3D(0.0f, 0.0f, 0.5f), Point3D(0.5f, 0.0f, 0.5f), Point3D(0.5f, 0.5f, 0.5f)),
                                             yellow));
}