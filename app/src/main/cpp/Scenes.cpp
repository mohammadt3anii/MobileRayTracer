//
// Created by Tiago on 05-12-2016.
//

#include "Scenes.h"

using namespace MobileRT;

Scene *cornellBoxScene() {
    Scene *scene = new Scene();
    // point light - white
    scene->lights.push_back(new PointLight(RGB(1.0f, 1.0f, 1.0f),
                                           Point3D(0.0f, 0.99f, 0.0f)));

    // back wall - white
    const Material lightGrayMat(RGB(0.9f, 0.9f, 0.9f));
    scene->primitives.push_back(new Primitive(new Plane(
            Point3D(0.0f, 0.0f, 1.0f), Vector3D(0.0f, 0.0f, -1.0f)), lightGrayMat));
    // floor - white
    scene->primitives.push_back(new Primitive(new Plane(
            Point3D(0.0f, -1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f)), lightGrayMat));
    // ceiling - white
    scene->primitives.push_back(new Primitive(new Plane(
            Point3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, -1.0f, 0.0f)), lightGrayMat));
    // left wall - red
    const Material redMat(RGB(0.9f, 0.0f, 0.0f));
    scene->primitives.push_back(new Primitive(new Plane(
            Point3D(-1.0f, 0.0f, 0.0f), Vector3D(1.0f, 0.0f, 0.0f)), redMat));
    // right wall - blue
    const Material blueMat(RGB(0.0f, 0.0f, 0.9f));
    scene->primitives.push_back(new Primitive(new Plane(
            Point3D(1.0f, 0.0f, 0.0f), Vector3D(-1.0f, 0.0f, 0.0f)), blueMat));

    // sphere - mirror
    const Material MirrorMat(RGB(0.0f, 0.0f, 0.0f), RGB(0.8f, 0.8f, 0.8f));
    scene->primitives.push_back(new Primitive(new Sphere(
            Point3D(0.45f, -0.65f, 0.4f), 0.35f), MirrorMat));

    // sphere - green
    const Material GreenMat(RGB(0.0f, 0.9f, 0.0f), RGB(0.0f, 0.2f, 0.0f));
    scene->primitives.push_back(new Primitive(new Sphere(
            Point3D(-0.45f, -0.1f, 0.0f), 0.35f), GreenMat));

    // triangle - yellow
    const Material yellowMat(RGB(1.0f, 1.0f, 0.0f));
    scene->primitives.push_back(new Primitive(new Triangle(
            Point3D(0.5f, -0.5f, 0.99f), Point3D(-0.5f, -0.5f, 0.99f),
            Point3D(0.5f, 0.5f, 1.001f)), yellowMat));
    return scene;
}

Scene *spheresScene() {
    Scene *scene = new Scene();
    // create one light source
    scene->lights.push_back(new PointLight(RGB(1.0f, 1.0f, 1.0f),
                                           Point3D(0.0f, 15.0f, 4.0f)));

    // create diffuse Materials
    Material sandMat(RGB(0.914f, 0.723f, 0.531f));
    Material redMat(RGB(0.9f, 0.0f, 0.0f));
    Material mirrorMat(RGB(0.0f, 0.0f, 0.0f), RGB(0.8f, 0.8f, 0.8f));
    Material greenMat(RGB(0.0f, 0.9f, 0.0f));
    // create one sphere
    scene->primitives.push_back(
            new Primitive(new Sphere(Point3D(-1.0f, 1.0f, 6.0f), 1.0f), redMat));
    scene->primitives.push_back(
            new Primitive(new Sphere(Point3D(1.5f, 2.0f, 7.0f), 1.0f), mirrorMat));
    scene->primitives.push_back(
            new Primitive(new Plane(Point3D(0.0f, 0.0f, 0.0f),
                                    Vector3D(0.0f, 1.0f, 0.0f)), sandMat));
    scene->primitives.push_back(
            new Primitive(new Sphere(Point3D(0.0f, 0.5f, 4.5f), 0.5f), greenMat));
    return scene;
}