//
// Created by Tiago on 16-10-2016.
//

#include "Spheres.h"
#include "../Shapes/Plane.h"
#include "../Shapes/Sphere.h"

using namespace MobileRT;

Spheres::Spheres() {
    // create one light source
    this->lights.push_back(new PointLight(RGB(1.0f, 1.0f, 1.0f), Point3D(0.0f, 15.0f, 4.0f)));

    // create diffuse Materials
    Material sandMat(RGB(0.914f, 0.723f, 0.531f));
    Material redMat(RGB(0.9f, 0.0f, 0.0f));
    Material mirrorMat(RGB(0.0f, 0.0f, 0.0f), RGB(0.8f, 0.8f, 0.8f));
    Material greenMat(RGB(0.0f, 0.9f, 0.0f));
    // create one sphere
    this->primitives.push_back(new Primitive(new Sphere(Point3D(-1.0f, 1.0f, 6.0f), 1.0f), redMat));
    this->primitives.push_back(
            new Primitive(new Sphere(Point3D(1.5f, 2.0f, 7.0f), 1.0f), mirrorMat));
    this->primitives.push_back(new Primitive(new Plane(Point3D(0.0f, 0.0f, 0.0f),
                                                       Vector3D(0.0f, 1.0f, 0.0f)), sandMat));
    this->primitives.push_back(new Primitive(new Sphere(Point3D(0.0f, 0.5f, 4.5f),
                                                        0.5f), greenMat));
}