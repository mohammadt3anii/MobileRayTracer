//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SCENESPHERES_H
#define MOBILERAYTRACER_SCENESPHERES_H

#include "Scene.h"

namespace MobileRT {
    class SceneSpheres : public Scene {
        SceneSpheres ();
    };
}

#endif //MOBILERAYTRACER_SCENESPHERES_H

/*
package com.example.lps.mobilert;
public class SceneSpheres extends Scene {
SceneSpheres () {
    // create one light source
    lights.add(new Light (new RGB(1.f, 1.f, 1.f), new myPoint(0.f, 15.f, 4.f)));

    // create diffuse Materials
    myMaterial sandMat = new myMaterial(new RGB(.914f, .723f, .531f));
    myMaterial redMat = new myMaterial(new RGB(0.9f, .0f, 0.f));
    myMaterial MirrorMat = new myMaterial(new RGB(), new RGB(.8f, .8f, .8f));
    myMaterial GreenMat = new myMaterial(new RGB(.0f, .9f, .0f));
    // create one sphere
    primitives.add(new Primitive(new Sphere(new myPoint(-1f, 1.f, 6.f)), redMat));
    primitives.add(new Primitive(new Sphere(new myPoint(1.5f, 2.f, 7.f)), MirrorMat));
    primitives.add(new Primitive(new Plane(new myPoint(0.f, 0.f, 0.f), new myVect(0.f, 1.f, 0.f)), sandMat));
    primitives.add(new Primitive(new Sphere(new myPoint(.0f, .5f, 4.5f), .5f), GreenMat));
}
}
*/