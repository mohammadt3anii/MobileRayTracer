//
// Created by puscas on 16-10-2016.
//

#ifndef MOBILERAYTRACER_SCENECORNELL_H
#define MOBILERAYTRACER_SCENECORNELL_H

#include "Scene.h"

namespace MobileRT {
    class SceneCornell : public Scene {
        SceneCornell ();
    };
}


#endif //MOBILERAYTRACER_SCENECORNELL_H
/*
package com.example.lps.mobilert;
public class SceneCornell extends Scene {
SceneCornell () {
    // create one light source
    lights.add(new Light (new RGB(1.f, 1.f, 1.f), new myPoint(0.f, 0.99f, 0.f)));

    // white Material
    myMaterial lightGrayMat = new myMaterial(new RGB(.9f, .9f, .9f));
    // back wall
    primitives.add(new Primitive(new Plane(new myPoint(0.f, 0.f, 1.f), new myVect(0.f, 0.f, -1.f)), lightGrayMat));
    // floor
    primitives.add(new Primitive(new Plane(new myPoint(0.f, -1.f, 0.f), new myVect(0.f, 1.f, 0.f)), lightGrayMat));
    // ceiling
    primitives.add(new Primitive(new Plane(new myPoint(0.f, 1.f, 0.f), new myVect(0.f, -1.f, 0.f)), lightGrayMat));
    // red Material
    myMaterial redMat = new myMaterial(new RGB(0.9f, .0f, 0.f));
    // left red wall
    primitives.add(new Primitive(new Plane(new myPoint(-1.f, 0.f, 0.f), new myVect(1.f, 0.f, 0.f)), redMat));
    // blue Material
    myMaterial blueMat = new myMaterial(new RGB(0.f, .0f, 0.9f));
    // left blue wall
    primitives.add(new Primitive(new Plane(new myPoint(1.f, 0.f, 0.f), new myVect(-1.f, 0.f, 0.f)), blueMat));


    myMaterial MirrorMat = new myMaterial(new RGB(), new RGB(.8f, .8f, .8f));
    // create one sphere
    primitives.add(new Primitive(new Sphere(new myPoint(.45f, -.65f, .4f), .35f), MirrorMat));
    myMaterial GreenMat = new myMaterial(new RGB(.0f, .9f, .0f), new RGB(0.f, 0.2f, 0.f));
    primitives.add(new Primitive(new Sphere(new myPoint(-.45f, -.1f, .0f), .35f), GreenMat));
}
}
*/